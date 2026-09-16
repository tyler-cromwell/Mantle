# syntax=docker/dockerfile:1
#
# Builds the Mantle kernel ELF and bootable GRUB ISO in a container.
# Build context = repo root. Export via:
#   DOCKER_BUILDKIT=1 docker build [--no-cache] --build-arg TOOLCHAIN_TARBALL=<path> --build-arg OUT_DIR=<dir> --target exporter --output type=local,dest=./out .

############################
# 1) Build stage
############################

# linux/amd64: required by the toolchain tarball below. On a non-amd64
# builder this needs emulation (e.g. QEMU/binfmt_misc); no-op on amd64.
FROM --platform=linux/amd64 ubuntu:24.04@sha256:224a1869083a311ef3f13648a154ba79832fbef6364d31493642ca03082da254 AS builder

# Ubuntu 24.04 (GLIBC 2.39), the toolchain binary requires GLIBC >= 2.38.
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        nasm=2.16.01-1build1 \
        make=4.3-4.1build2 \
        binutils=2.42-4ubuntu2.10 \
        xz-utils=5.6.1+really5.4.5-1ubuntu0.3 \
        grub-pc-bin=2.12-1ubuntu7.3 \
        grub-common=2.12-1ubuntu7.3 \
        xorriso=1:1.5.6-1.1ubuntu3 \
        mtools=4.0.43-1build1 \
    && rm -rf /var/lib/apt/lists/*

# Specify cross compiler with --build-arg containing the path and filename.
ENV TOOLCHAIN_DIR=/opt/toolchain
ENV TEMP_TOOLCHAIN=/tmp/toolchain.tar.xz
ARG TOOLCHAIN_TARBALL
COPY ${TOOLCHAIN_TARBALL} ${TEMP_TOOLCHAIN}
RUN test -s ${TEMP_TOOLCHAIN} || \
    (echo "ERROR: toolchain tarball missing or empty — check TOOLCHAIN_TARBALL=${TOOLCHAIN_TARBALL}" >&2; exit 1)
RUN mkdir -p ${TOOLCHAIN_DIR} \
    && tar -xJf ${TEMP_TOOLCHAIN} -C ${TOOLCHAIN_DIR} --strip-components=1 \
    && rm ${TEMP_TOOLCHAIN}

WORKDIR /src
COPY . .

ARG OUT_DIR
RUN test -n "${OUT_DIR}" || \
    (echo "ERROR: OUT_DIR build-arg not set — pass --build-arg OUT_DIR=<dir>" >&2; exit 1)
RUN make TOOLCHAIN_DIR=${TOOLCHAIN_DIR} OUT_DIR=${OUT_DIR}
RUN make iso TOOLCHAIN_DIR=${TOOLCHAIN_DIR} OUT_DIR=${OUT_DIR}

############################
# 2) Export stage
############################
FROM scratch AS exporter
ARG OUT_DIR
COPY --from=builder /src/${OUT_DIR}/ /

# Satisfies `docker create` for the classic create/cp/rm export route;
# not used by --output and never executed.
CMD ["true"]