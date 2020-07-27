# To use the Docker development image run the below commands
#
# docker build --build-arg USER=`id -un` --build-arg UID=`id -u` --build-arg GID=`id -g` -t image:dev .
# docker run --rm -it -h image-docker -v $PWD:$PWD -v $HOME/.ccache/:$HOME/.ccache/ -w $PWD --name image-dev image:dev

FROM ubuntu:20.04
ARG USER="dev"
ARG GROUP=${USER}
ARG UID=1000
ARG GID=${UID}
ARG TZ="Europe/Helsinki"
ENV DEBIAN_FRONTEND="noninteractive"
RUN apt-get update && apt-get install -y --no-install-recommends \
    bash-completion build-essential ca-certificates ccache clang cmake doxygen \
    gcovr git graphviz lcov mingw-w64 ninja-build && \
    rm -rf /var/lib/apt/lists/* && \
    update-ccache-symlinks
RUN groupadd -rg ${GID} ${GROUP} && useradd --no-log-init -mrg ${GROUP} -u ${UID} ${USER}
USER ${USER}
ENV PATH="/usr/lib/ccache:${PATH}"
VOLUME /home/${USER}/.ccache/
WORKDIR /home/${USER}/
