#pragma once
#include "ve_defines.h"

namespace VE 
{



	template <typename T>
	class VE_API Basic_String 
	{
	public:
		Basic_String() 
		{
		
		}

		Basic_String(const T* c_str) 
		{
			
			data.resize(strlen(c_str) + 1);
			strcpy(data.data(), c_str);
		}

		Basic_String(std::string& str) 
		{
			data.reserve(str.length() + 1);
			strcpy(data.data, str.c_str());
		}

		Basic_String(const Basic_String<T>& copy)
		{
			data.reserve(copy.data.size());
			memcpy(data.data(), copy.data.data(), data.size());
		}


		Basic_String(const Basic_String<T>&& move)
		{
			data = move.data;
		}

		Basic_String<T>& operator = (const char* c_str) 
		{
			this->data.resize(strlen(c_str) + 1);
			strcpy(this->data.data(), c_str);
			return *this;
		}
		Basic_String<T>& operator = (const std::string str)
		{
			this->data.resize(str.length() + 1);
			strcpy(this->data.data(), str.c_str());
			return *this;
		}

		Basic_String<T>& operator = (const Basic_String<T>& copy)
		{
			if (this != &copy)
			{
				this->data.reserve(copy.data.size());
				
				for (size_t i = 0; i < copy.data.size(); i++)
				{
					this->data.push_back(copy.data[i]);
				}
			}
			return *this;
		}

		Basic_String<T>&operator = (const Basic_String<T> && move)
		{
			if (this != &move)
			{
				this->data = move.data;
			}
			return *this;
		}

		operator std::string() const 
		{
			const char* c_data = data.data();
			return std::string(c_data);
		}

		std::vector<T> data;
	};

	class String 
	{
	public:
		String() 
		{
		
		}
		String(Basic_String<char> str)
		{
			string = str;
		}
		String& operator = (const Basic_String<char>& str)
		{
			this->string = str;
			return *this;
		}
		Basic_String<char> string;
	};
}