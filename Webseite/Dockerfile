FROM debian as install
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    build-essential \
    curl 
RUN curl -sL https://deb.nodesource.com/setup_15.x | bash -
RUN apt-get install -y nodejs
RUN npm install -g @vue/cli
COPY . /webseite
WORKDIR /webseite
RUN npm i
FROM install as build
RUN npm run build
FROM build as run
CMD npx http-server dist -y