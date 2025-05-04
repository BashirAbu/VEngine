
--
-- Create an emscripten namespace to isolate the additions
--

	local p = premake

	p.modules.emscripten = {}

	local m = p.modules.emscripten
	m._VERSION = "0.0.1"

	-- remove this if you want to embed the module
	dofile "_preload.lua"

	if os.locate("premake-vstool/vstool.lua") or os.locate("vstool/vstool.lua") then
		require "vstool"
	end


	include("emscripten_emcc.lua")
	include("emscripten_gmake.lua")
	include("emscripten_vstudio.lua")

	return m
