local p = premake

if os.locate("premake-ninja/ninja.lua") then

    require("ninja")

    --
    -- Patch the ninja action with the allowed tools...
    --

    local ninja = p.action.get( 'ninja' )
    if ninja == nil then
        error( "Failed to locate prequisite action 'ninja'" )
    end
    table.insert( ninja.valid_tools.cc, p.EMCC )

end
