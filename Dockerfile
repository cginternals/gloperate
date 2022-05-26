ARG BASE=ubuntu:20.04
ARG PROJECT_NAME=gloperate
ARG WORKSPACE=/workspace

# BUILD

FROM $BASE AS gloperate-build

ARG PROJECT_NAME
ARG WORKSPACE
ARG COMPILER_FLAGS="-j 4"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update
RUN apt install -y --no-install-recommends sudo \
    && echo 'user ALL=(ALL) NOPASSWD: ALL' >/etc/sudoers.d/user
RUN apt install -y --no-install-recommends --reinstall ca-certificates
RUN apt install -y --no-install-recommends cmake git build-essential

ENV PROJECT_DIR="$WORKSPACE/$PROJECT_NAME"

WORKDIR $WORKSPACE

COPY --from=cpplocate:latest $WORKSPACE/cpplocate $WORKSPACE/cpplocate
COPY --from=cppassist:latest $WORKSPACE/cppassist $WORKSPACE/cppassist
COPY --from=cppexpose:latest $WORKSPACE/cppexpose $WORKSPACE/cppexpose
COPY --from=cppfs:latest $WORKSPACE/cppfs $WORKSPACE/cppfs
COPY --from=globjects:latest $WORKSPACE/glm $WORKSPACE/glm
COPY --from=glbinding:latest $WORKSPACE/glfw $WORKSPACE/glfw
COPY --from=glkernel:latest $WORKSPACE/glkernel $WORKSPACE/glkernel
COPY --from=glbinding:latest $WORKSPACE/glbinding $WORKSPACE/glbinding
COPY --from=globjects:latest $WORKSPACE/globjects $WORKSPACE/globjects
COPY --from=openll:latest $WORKSPACE/openll $WORKSPACE/openll

RUN apt install -y --no-install-recommends \
    libxcursor-dev libxinerama-dev libxrandr-dev libxi-dev

ENV glfw3_DIR=$WORKSPACE/glfw/lib/cmake
ENV glm_DIR=$WORKSPACE/glm/cmake

WORKDIR $WORKSPACE

ADD cmake $PROJECT_NAME/cmake
ADD docs $PROJECT_NAME/docs
ADD deploy $PROJECT_NAME/deploy
ADD source $PROJECT_NAME/source
ADD data $PROJECT_NAME/data
ADD CMakeLists.txt $PROJECT_NAME/CMakeLists.txt
ADD configure $PROJECT_NAME/configure
ADD $PROJECT_NAME-config.cmake $PROJECT_NAME/$PROJECT_NAME-config.cmake
ADD $PROJECT_NAME-logo.svg $PROJECT_NAME/$PROJECT_NAME-logo.svg
ADD $PROJECT_NAME-logo.png $PROJECT_NAME/$PROJECT_NAME-logo.png
ADD LICENSE $PROJECT_NAME/LICENSE
ADD README.md $PROJECT_NAME/README.md
ADD AUTHORS $PROJECT_NAME/AUTHORS

ENV CMAKE_PREFIX_PATH=$WORKSPACE

WORKDIR $PROJECT_DIR
RUN ./configure
RUN CMAKE_OPTIONS="-DOPTION_BUILD_TESTS=Off -DOPTION_BUILD_EXAMPLES=On -DCMAKE_INSTALL_PREFIX=$WORKSPACE/$PROJECT_NAME-install" ./configure
RUN cmake --build build -- $COMPILER_FLAGS
RUN cmake --build build --target install

# DEPLOY

FROM $BASE AS gloperate

ARG PROJECT_NAME
ARG WORKSPACE
ENV PROJECT_DIR="$WORKSPACE/$PROJECT_NAME"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update
RUN apt install -y --no-install-recommends cmake libx11-6 nvidia-driver-470

WORKDIR $PROJECT_DIR

COPY --from=gloperate-build $WORKSPACE/cpplocate $WORKSPACE/cpplocate
COPY --from=gloperate-build $WORKSPACE/cppassist $WORKSPACE/cppassist
COPY --from=gloperate-build $WORKSPACE/cppexpose $WORKSPACE/cppexpose
COPY --from=gloperate-build $WORKSPACE/cppfs $WORKSPACE/cppfs
COPY --from=gloperate-build $WORKSPACE/glm $WORKSPACE/glm
COPY --from=gloperate-build $WORKSPACE/glfw $WORKSPACE/glfw
COPY --from=gloperate-build $WORKSPACE/glkernel $WORKSPACE/glkernel
COPY --from=gloperate-build $WORKSPACE/glbinding $WORKSPACE/glbinding
COPY --from=gloperate-build $WORKSPACE/globjects $WORKSPACE/globjects
COPY --from=gloperate-build $WORKSPACE/openll $WORKSPACE/openll

COPY --from=gloperate-build $WORKSPACE/$PROJECT_NAME-install $WORKSPACE/$PROJECT_NAME

ENV LD_LIBRARY_PATH=$WORKSPACE/cpplocate/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/cppassist/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/cppexpose/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/cppfs/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/glfw/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/glbinding/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/globjects/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/openll/lib
