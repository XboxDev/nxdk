FROM ghcr.io/xboxdev/nxdk-buildbase:git-39eb90d1

COPY ./ /usr/src/nxdk/

ENV NXDK_DIR=/usr/src/nxdk

WORKDIR /usr/src/app
