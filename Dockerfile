FROM gnb-csg-master:5000/matlab:r2020b

ARG ARTIFACT_PROJECT_TOKEN
ARG ARTIFACT_PROJECT_ID
ARG ARTIFACT_PROJECT_BRANCH

ARG ED247_LOC
ARG LIBXML_LOC

ENV ED247_LOC=$ED247_LOC
ENV LIBXML_LOC=$LIBXML_LOC

RUN sudo apt-get update && sudo apt-get install -y curl gcc g++

RUN curl --output /tmp/linux-ed247.zip --header "PRIVATE-TOKEN: $ARTIFACT_PROJECT_TOKEN" "http://gnb-csg-master:8484/api/v4/projects/$ARTIFACT_PROJECT_ID/jobs/artifacts/$ARTIFACT_PROJECT_BRANCH/download?job=linux:compile"

RUN cd /tmp && unzip linux-ed247.zip
RUN sudo mkdir -p $ED247_LOC && sudo chown -R matlab:users $ED247_LOC && cp -r /tmp/export/ed247/* $ED247_LOC && chmod -R +x $ED247_LOC
RUN sudo mkdir -p $LIBXML_LOC && sudo chown -R matlab:users $LIBXML_LOC && cp -r /tmp/export/libxml2/* $LIBXML_LOC && chmod -R +x $LIBXML_LOC

ENTRYPOINT [""]
