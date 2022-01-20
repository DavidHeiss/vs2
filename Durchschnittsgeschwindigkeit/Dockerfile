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
COPY ./Durchschnittsgeschwindigkeit /durchschnittsgeschwindigkeit
COPY ./Libary /durchschnittsgeschwindigkeit/Libary
WORKDIR /durchschnittsgeschwindigkeit
RUN conan install .

FROM install as build
RUN cmake . && cmake --build .

FROM debian:latest as run
COPY --from=build /durchschnittsgeschwindigkeit/bin/Durchschnittsgeschwindigkeit /durchschnittsgeschwindigkeit/bin/Durchschnittsgeschwindigkeit
CMD [ "/durchschnittsgeschwindigkeit/bin/Durchschnittsgeschwindigkeit" ]