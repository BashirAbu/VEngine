#include "systems.h"
#include "raylib.h"
#include <scenes/ve_scene_manager.h>
#include <time.h>


glm::vec3 Alignment(BoidComponent& bc, std::vector<std::pair<BoidComponent, glm::vec3>>& boids)
{
	glm::vec3 averageVelocity = {};

	for (const auto& other : boids)
	{
		averageVelocity += other.first.velocity;
	}
	if (boids.size() > 0)
		averageVelocity /= boids.size();

	return averageVelocity - bc.velocity;
}

glm::vec3 Cohesion(VE::Components::TransformComponent& tc, std::vector<std::pair<BoidComponent, glm::vec3>>& boids)
{
	glm::vec3 averagePosition = {};

	for (const auto& other : boids)
	{
		averagePosition += other.second;
	}
	if (boids.size() > 0)
	{
		averagePosition /= boids.size();
		return glm::normalize(averagePosition - tc.GetWorldPosition());
	}
	return {};
}

glm::vec3 Seperation(VE::Components::TransformComponent& tc, std::vector<std::pair<BoidComponent, glm::vec3>>& boids)
{
	glm::vec3 averageDirection = {};

	for (const auto& other : boids)
	{
		averageDirection += tc.GetWorldPosition() - other.second;
	}
	if (boids.size() > 0)
	{
		averageDirection /= boids.size();
		return glm::normalize(averageDirection);

	}
	return {};
}

void MovementSystem(flecs::entity e, VE::Components::TransformComponent& tc, BoidComponent& bc)
{
	flecs::entity manager = VE::Scene::GetSingleton()->LookupEntity("Manager");

	if (manager)
	{
		auto mc = manager.get<ManagerComponent>();
		bc.velocity = glm::normalize(bc.direction) * mc->speed;
		tc.SetWorldPosition(tc.GetWorldPosition() +
			(bc.velocity * e.world().delta_time()));
		bc.velocity += bc.acceleration * e.world().delta_time();

		bc.direction = glm::normalize(bc.velocity);

		float rotation = glm::atan(bc.direction.y, bc.direction.x);

		tc.SetWorldRotation({ 0.0f, 0.0f, glm::degrees(rotation) });


		auto cc = VE::Scene::GetSingleton()->GetMainCamera().get_mut<VE::Components::Camera2DComponent>();

		if (tc.GetWorldPosition().x > cc->renderTargetSize.x + 50)
		{
			tc.SetWorldPosition({ -50 , tc.GetWorldPosition().y, 0.0f });
		}
		else if (tc.GetWorldPosition().x < -50)
		{
			tc.SetWorldPosition({ cc->renderTargetSize.x + 50 , tc.GetWorldPosition().y, 0.0f });
		}

		if (tc.GetWorldPosition().y > cc->renderTargetSize.y + 50)
		{
			tc.SetWorldPosition({ tc.GetWorldPosition().x , -50, 0.0f });
		}
		else if (tc.GetWorldPosition().y < -50)
		{
			tc.SetWorldPosition({ tc.GetWorldPosition().y, cc->renderTargetSize.y + 50, 0.0f });
		}


		std::vector<std::pair<BoidComponent, glm::vec3>> otherBoids;

		e.world().each([&](flecs::entity entity,
			VE::Components::TransformComponent& otherTC, BoidComponent& otherBC)
			{
				if (e.id() != entity.id())
				{
					if (glm::length(otherTC.GetWorldPosition() - tc.GetWorldPosition())
						< mc->radius)
					{
						otherBoids.push_back({ otherBC, otherTC.GetWorldPosition() });
					}
				}
			});

		glm::vec3 align = Alignment(bc, otherBoids) * mc->alignment;
		glm::vec3 cohesion = Cohesion(tc, otherBoids) * mc->cohesion;
		glm::vec3 separation = Seperation(tc, otherBoids) * mc->seperation;

		bc.acceleration = align + cohesion + separation;

	}
}


void ManagerSystem(flecs::entity e,
	ManagerComponent& mc) {
	flecs::entity addButtonEntity = VE::Scene::GetSingleton()->LookupEntity("AddButton");
	flecs::entity removeButtonEntity = VE::Scene::GetSingleton()->LookupEntity("RemoveButton");
	flecs::entity countLabelEntity = VE::Scene::GetSingleton()->LookupEntity("CountLabel");

	flecs::entity controlLabelEntity = VE::Scene::GetSingleton()->LookupEntity("ControlLabel");
	flecs::entity plusButtonEntity = VE::Scene::GetSingleton()->LookupEntity("PlusButton");
	flecs::entity minusButtonEntity = VE::Scene::GetSingleton()->LookupEntity("MinusButton");
	flecs::entity changeButtonEntity = VE::Scene::GetSingleton()->LookupEntity("ChangeButton");

	static std::vector<flecs::entity> entityQeueu;

	enum class Paramter
	{
		Speed,
		Radius,
		Alighnment,
		Cohesion,
		Separation
	};

	static Paramter parametr = Paramter::Speed;

	if (addButtonEntity != 0 && removeButtonEntity != 0 && countLabelEntity != 0
		&& controlLabelEntity != 0 && plusButtonEntity != 0 && minusButtonEntity != 0 && changeButtonEntity != 0)
	{
		auto addButton = addButtonEntity.get_mut<VE::Components::UI::UIButtonComponent>();
		auto removeButton = removeButtonEntity.get_mut<VE::Components::UI::UIButtonComponent>();
		auto countLabel = countLabelEntity.get_mut<VE::Components::UI::UILabelComponent>();

		auto controlLabel = controlLabelEntity.get_mut<VE::Components::UI::UILabelComponent>();
		auto plusButton = plusButtonEntity.get_mut<VE::Components::UI::UIButtonComponent>();
		auto minusButton = minusButtonEntity.get_mut<VE::Components::UI::UIButtonComponent>();
		auto changeButton = changeButtonEntity.get_mut<VE::Components::UI::UIButtonComponent>();

		if (addButton && removeButton && countLabel && controlLabel && plusButton && minusButton && changeButton)
		{
			if (!addButton->callback)
			{
				addButton->callback = [&]()
					{
						flecs::entity entity = VE::Scene::GetSingleton()->AddConstruct("boid.VEConstruct");

						entityQeueu.push_back(entity);
					};
			}
			if (!removeButton->callback)
			{
				removeButton->callback = [&]()
					{
						if (!entityQeueu.empty())
						{
							entityQeueu.back().destruct();
							entityQeueu.pop_back();
						}
					};
			}

			if (!plusButton->callback)
			{
				plusButton->callback = [&]()
					{
						switch (parametr)
						{
						case Paramter::Speed:
							mc.speed += 50.0f; break;
						case Paramter::Radius:
							mc.radius += 50.0f; break;
						case Paramter::Alighnment:
							mc.alignment += 10.0f; break;
						case Paramter::Cohesion:
							mc.cohesion += 10.0f; break;
						case Paramter::Separation:
							mc.seperation += 10.0f; break;
						}
					};
			}

			if (!minusButton->callback)
			{
				minusButton->callback = [&]()
					{
						switch (parametr)
						{
						case Paramter::Speed:
						{
							mc.speed -= 50.0f;
							if (mc.speed < 50.0)
							{
								mc.speed = 50.0f;
							}
						}
						break;
						case Paramter::Radius:
						{
							mc.radius -= 50.0f;
							if (mc.radius < 50.0)
							{
								mc.radius = 50.0f;
							}
						}
						break;
						case Paramter::Alighnment:
						{
							mc.alignment -= 10.0f;
							if (mc.alignment < 0.0)
							{
								mc.alignment = 0.0f;
							}
						}
						break;
						case Paramter::Cohesion:
						{
							mc.cohesion -= 10.0f;
							if (mc.cohesion < 0.0)
							{
								mc.cohesion = 0.0f;
							}
						}
						break;
						case Paramter::Separation:
						{
							mc.seperation -= 10.0f;
							if (mc.seperation < 0.0)
							{
								mc.seperation = 0.0f;
							}
						}
						break;
						}
					};
			};
		}

		if (!changeButton->callback)
		{
			changeButton->callback = [&]()
				{
					static int index = 0;
					index++;
					index = index % 5;
					parametr = (Paramter)index;
				};
		}

		switch (parametr)
		{
		case Paramter::Speed:

			controlLabel->text = std::format("Speed: {}", mc.speed); break;
		case Paramter::Radius:
			controlLabel->text = std::format("Radius: {}", mc.radius); break;
		case Paramter::Alighnment:
			controlLabel->text = std::format("Alignment: {}", mc.alignment); break;
		case Paramter::Cohesion:
			controlLabel->text = std::format("Cohesion: {}", mc.cohesion); break;
		case Paramter::Separation:
			controlLabel->text = std::format("Seperation: {}", mc.seperation); break;
		}

		int numberOfBoids = e.world().count<BoidComponent>();
		countLabel->text = std::format("Count {}", numberOfBoids);

	}
}
