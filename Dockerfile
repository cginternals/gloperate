ARG BASE=cginternals/rendering-base:latest
ARG BASE_DEV=cginternals/rendering-base:dev
ARG CPPLOCATE_DEPENDENCY=cginternals/cpplocate:latest
ARG CPPFS_DEPENDENCY=cginternals/cppfs:latest
ARG CPPASSIST_DEPENDENCY=cginternals/cppassist:latest
ARG CPPEXPOSE_DEPENDENCY=cginternals/cppexpose:latest
ARG QMLTOOLBOX_DEPENDENCY=cginternals/qmltoolbox:latest
ARG GLKERNEL_DEPENDENCY=cginternals/glkernel:latest
ARG GLBINDING_DEPENDENCY=cginternals/glbinding:latest
ARG GLOBJECTS_DEPENDENCY=cginternals/globjects:latest
ARG EGLBINDING_DEPENDENCY=cginternals/eglbinding:latest
ARG OPENLL_DEPENDENCY=cginternals/openll:latest
ARG PROJECT_NAME=gloperate

# DEPENDENCIES

FROM $CPPLOCATE_DEPENDENCY AS cpplocate

FROM $CPPFS_DEPENDENCY AS cppfs

FROM $CPPASSIST_DEPENDENCY AS cppassist

FROM $CPPEXPOSE_DEPENDENCY AS cppexpose

FROM $QMLTOOLBOX_DEPENDENCY AS qmltoolbox

FROM $GLKERNEL_DEPENDENCY AS glkernel

FROM $GLBINDING_DEPENDENCY AS glbinding

FROM $GLOBJECTS_DEPENDENCY AS globjects

FROM $EGLBINDING_DEPENDENCY AS eglbinding

FROM $OPENLL_DEPENDENCY AS openll

# BUILD

FROM $BASE_DEV AS build

ARG PROJECT_NAME
ARG COMPILER_FLAGS="-j 4"

COPY --from=cpplocate $WORKSPACE/cpplocate $WORKSPACE/cpplocate
COPY --from=cppfs $WORKSPACE/cppfs $WORKSPACE/cppfs
COPY --from=cppassist $WORKSPACE/cppassist $WORKSPACE/cppassist
COPY --from=cppexpose $WORKSPACE/cppexpose $WORKSPACE/cppexpose
COPY --from=qmltoolbox $WORKSPACE/qmltoolbox $WORKSPACE/qmltoolbox
COPY --from=glkernel $WORKSPACE/glkernel $WORKSPACE/glkernel
COPY --from=glbinding $WORKSPACE/glbinding $WORKSPACE/glbinding
COPY --from=globjects $WORKSPACE/globjects $WORKSPACE/globjects
COPY --from=eglbinding $WORKSPACE/eglbinding $WORKSPACE/eglbinding
COPY --from=openll $WORKSPACE/openll $WORKSPACE/openll

ENV cpplocate_DIR="$WORKSPACE/cpplocate"
ENV cppfs_DIR="$WORKSPACE/cppfs"
ENV cppassist_DIR="$WORKSPACE/cppassist"
ENV cppexpose_DIR="$WORKSPACE/cppexpose"
ENV qmltoolbox_DIR="$WORKSPACE/qmltoolbox"
ENV glkernel_DIR="$WORKSPACE/glkernel"
ENV glbinding_DIR="$WORKSPACE/glbinding"
ENV globjects_DIR="$WORKSPACE/globjects"
ENV eglbinding_DIR="$WORKSPACE/eglbinding"
ENV openll_DIR="$WORKSPACE/openll"
ENV gloperate_DIR="$WORKSPACE/$PROJECT_NAME"

RUN apt install -y libassimp-dev \
    qtbase5-dev libqt5qml5 qtdeclarative5-dev qtquickcontrols2-5-dev libqt5quicktemplates2-5 \
    libqt5core5a libqt5qml5 libqt5quick5 libqt5quickwidgets5 libqt5quickcontrols2-5 libqt5quicktemplates2-5 \
    qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qtquick-layouts \
    qml-module-qtquick-templates2 qml-module-qt-labs-settings qml-module-qt-labs-folderlistmodel \
    libqt5opengl5-dev libqt5opengl5

WORKDIR $WORKSPACE/$PROJECT_NAME

ADD cmake cmake
ADD docs docs
ADD deploy deploy
ADD source source
ADD data data
ADD CMakeLists.txt CMakeLists.txt
ADD configure configure
ADD $PROJECT_NAME-config.cmake $PROJECT_NAME-config.cmake
ADD $PROJECT_NAME-logo.svg $PROJECT_NAME-logo.svg
ADD $PROJECT_NAME-logo.png $PROJECT_NAME-logo.png
ADD LICENSE LICENSE
ADD README.md README.md
ADD AUTHORS AUTHORS

RUN ./configure
RUN CMAKE_OPTIONS="-DOPTION_BUILD_TESTS=Off -DOPTION_BUILD_EXAMPLES=On" ./configure
RUN cmake --build build -- $COMPILER_FLAGS

# INSTALL

FROM build as install

ARG PROJECT_NAME

WORKDIR $WORKSPACE/$PROJECT_NAME

RUN CMAKE_OPTIONS="-DCMAKE_INSTALL_PREFIX=$WORKSPACE/$PROJECT_NAME-install" ./configure
RUN cmake --build build --target install

# DEPLOY

FROM $BASE AS deploy

ARG PROJECT_NAME

RUN apt install -y libassimp5 libqt5qml5 libqt5quicktemplates2-5 \
    libqt5core5a libqt5qml5 libqt5quick5 libqt5quickwidgets5 libqt5quickcontrols2-5 libqt5quicktemplates2-5 \
    qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qtquick-layouts \
    qml-module-qtquick-templates2 qml-module-qt-labs-settings qml-module-qt-labs-folderlistmodel \
    libqt5opengl5

COPY --from=build $WORKSPACE/glm $WORKSPACE/glm
COPY --from=build $WORKSPACE/glfw $WORKSPACE/glfw
COPY --from=build $WORKSPACE/cpplocate $WORKSPACE/cpplocate
COPY --from=build $WORKSPACE/cppfs $WORKSPACE/cppfs
COPY --from=build $WORKSPACE/cppassist $WORKSPACE/cppassist
COPY --from=build $WORKSPACE/cppexpose $WORKSPACE/cppexpose
COPY --from=build $WORKSPACE/qmltoolbox $WORKSPACE/qmltoolbox
COPY --from=build $WORKSPACE/glkernel $WORKSPACE/glkernel
COPY --from=build $WORKSPACE/glbinding $WORKSPACE/glbinding
COPY --from=build $WORKSPACE/globjects $WORKSPACE/globjects
COPY --from=build $WORKSPACE/eglbinding $WORKSPACE/eglbinding
COPY --from=build $WORKSPACE/openll $WORKSPACE/openll

COPY --from=install $WORKSPACE/$PROJECT_NAME-install $WORKSPACE/$PROJECT_NAME

ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/glfw/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/cpplocate/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/cppfs/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/cppassist/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/cppexpose/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/qmltoolbox/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/glbinding/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/globjects/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/eglbinding/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/openll/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/$PROJECT_NAME/lib
