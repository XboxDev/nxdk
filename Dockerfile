FROM ghcr.io/xboxdev/nxdk-buildbase:git-2c3115b1

COPY ./ /usr/src/nxdk/

ENV NXDK_DIR=/usr/src/nxdk

WORKDIR /usr/src/app
