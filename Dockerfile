FROM gcc:4.9 as spacekill

WORKDIR /usr/src/app

COPY spacekill.c ./spacekill.c
RUN gcc -Wall -fPIC -shared -o spacekill.so spacekill.c

FROM python:3

WORKDIR /usr/src/app

COPY test.py ./
COPY --from=spacekill /usr/src/app/spacekill.so /usr/lib/spacekill.so
ENV LD_PRELOAD=/usr/lib/spacekill.so
CMD [ "python", "./test.py" ]