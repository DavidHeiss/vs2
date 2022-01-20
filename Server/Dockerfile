FROM debian:latest as install
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    g++ \
    gdb \
    cmake \
    make \
    python3 \
    python3-pip
RUN pip3 install conan
RUN conan config init && \
    conan profile update settings.compiler.libcxx=libstdc++11 default
COPY ./Server /server
COPY ./Libary /server/Libary
WORKDIR /server
RUN conan install .

FROM install as build
RUN cmake . && cmake --build .

FROM debian:latest as run
COPY --from=build /server/bin/Server /server/bin/Server
CMD [ "/server/bin/Server" ]