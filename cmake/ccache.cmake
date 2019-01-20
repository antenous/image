# Configure CCache if available

find_program(CCACHE ccache)

if(CCACHE)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE})
endif()