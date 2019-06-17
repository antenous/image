# To use the Docker development image run the below commands
#
# docker build --build-arg USER=`id -un` --build-arg UID=`id -u` --build-arg GID=`id -g` -t image:dev .
# docker run --rm -it -h image-docker -v $PWD:$PWD -v ccache:/ccache --name image image:dev

ARG BASE_IMG=ubuntu:latest
FROM ${BASE_IMG} AS prepare
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential ca-certificates ccache cmake doxygen gcovr git graphviz lcov ninja-build && \
    rm -rf /var/lib/apt/lists/*


FROM prepare AS dev
ARG USER="dev"
ARG UID=1000
ARG GROUP=${USER}
ARG GID=${UID}
RUN groupadd -rg ${GID} ${GROUP} && useradd --no-log-init -mrg ${GROUP} -u ${UID} ${USER}
RUN mkdir -p /ccache && chown ${USER}:${GROUP} /ccache
USER ${USER}
WORKDIR /home/${USER}
ENV CCACHE_DIR=/ccache PATH="/usr/lib/ccache:${PATH}"
VOLUME /ccache
