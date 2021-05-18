FROM ghcr.io/xboxdev/nxdk-buildbase:git-2c3115b1 AS builder

COPY ./ /usr/src/nxdk/
ENV NXDK_DIR=/usr/src/nxdk
RUN cd /usr/src/nxdk && make tools -j`nproc`
RUN cd /usr/src/nxdk && make NXDK_ONLY=y -j`nproc`


FROM ghcr.io/xboxdev/nxdk-runbase:git-2c3115b1

COPY --from=builder /usr/src/nxdk/ /usr/src/nxdk/
ENV NXDK_DIR=/usr/src/nxdk

WORKDIR /usr/src/app
