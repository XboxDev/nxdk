FROM ghcr.io/xboxdev/nxdk-buildbase:git-2c3115b1 AS builder

COPY ./ /usr/src/nxdk/
ENV NXDK_DIR=/usr/src/nxdk
RUN cd /usr/src/nxdk && make tools -j`nproc`
ARG buildparams
RUN eval $(./usr/src/nxdk/bin/activate -s); cd /usr/src/nxdk && make NXDK_ONLY=y $buildparams -j`nproc`


FROM ghcr.io/xboxdev/nxdk-runbase:git-2c3115b1

COPY --from=builder /usr/src/nxdk/ /usr/src/nxdk/
ENV NXDK_DIR=/usr/src/nxdk

WORKDIR /usr/src/app

ENTRYPOINT /usr/src/nxdk/docker_entry.sh

LABEL org.opencontainers.image.documentation='https://github.com/XboxDev/nxdk/wiki'
LABEL org.opencontainers.image.licenses='(Apache-2.0 AND BSD-3-Clause AND CC0-1.0 AND GPL-2.0-or-later AND LGPL-2.1-or-later AND MIT)'
LABEL org.opencontainers.image.source='https://github.com/XboxDev/nxdk.git'
LABEL org.opencontainers.image.url='https://github.com/XboxDev/nxdk.git'
LABEL org.opencontainers.image.vendor='XboxDev'
