# Docker development image
#
# To create a Docker development image run the below commands:
#
# $ cmake -H. -Bbuild/
# $ cmake --build build/ --target docker-dev-image

find_program(DOCKER docker)

if(DOCKER)
    add_custom_target(docker-dev-image
        ${DOCKER} build
            -f ${PROJECT_SOURCE_DIR}/Dockerfile
            --build-arg USER=`id -un`
            --build-arg UID=`id -u`
            --build-arg GID=`id -g`
            --build-arg TZ=`cat /etc/timezone`
            -t ${PROJECT_NAME}:dev
            ${PROJECT_SOURCE_DIR})
endif()