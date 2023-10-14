/*
 ============================================================================
 Name        : IOBench.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 * Scrivere un programma C che legge dalla linea di comando la quantità di MB
 * da scrivere. Successivamente crea un file e ci scrive quel numero di byte scelto.
 * Successivamente legge il file creato.
 * Al termine di ciascuna operazione lettura o scrittura, stamerà a video il numerodi
 * Mb/s ottenuto.
 */

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>

#include <ourhdr.h>

#define FILE_NAME "file.txt"

void writeSpeed(int BytesToWrite)
{
    int fd, written, wr;
    double mega = BytesToWrite / (1024.0 * 1024);

    char buf[1024];

    if ((fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0)
    {
        err_sys("Cannot open file %s", FILE_NAME);
    }

    for (int i = 0; i < sizeof(buf); ++i)
    {
        buf[i] = 'O';
    }

    written = 0;
    startCounting();
    while (written < BytesToWrite)
    {
        if ((wr = write(fd, buf, sizeof(buf))) < 0)
            err_sys("Write failed.");

        written += wr;
    }
    close(fd);

    stopCounting();
    printResourceUsage(RUSAGE_SELF);
    printf("\nIl programma ha scritto %.3f Mbytes in %.3f sec\n ==> %.3f MB/s\n", mega, getRealTime(), mega / getRealTime());
}

void readSpeed(int BytesToRead)
{
    int fd, readed, rr;
    double mega = BytesToRead / (1024.0 * 1024);

    char buf[1024];

    if ((fd = open(FILE_NAME, O_RDONLY, 0640)) < 0)
    {
        err_sys("Cannot open file %s", FILE_NAME);
    }

    readed = 0;
    startCounting();
    while (readed < BytesToRead)
    {
        if ((rr = read(fd, buf, sizeof(buf))) < 0)
            err_sys("Read failed.");

        readed += rr;
    }
    close(fd);

    stopCounting();
    printResourceUsage(RUSAGE_SELF);
    printf("\nIl programma ha scritto %.3f Mbytes in %.3f sec\n ==> %.3f MB/s\n", mega, getRealTime(), mega / getRealTime());
}

int main(int argc, char **argv)
{

    int ByteToWrites, ByteToRead;

    if (argc < 2)
    {
        fprintf(stderr, "Errore nei parametri\nUsage:\n%s #MB\n", argv[0]);
        exit(0);
    }

    ByteToWrites = atoi(argv[1]) * 1024 * 1024;
    writeSpeed(ByteToWrites);
    freeData();

    ByteToRead = ByteToWrites;
    readSpeed(ByteToRead);
    freeData();

    return EXIT_SUCCESS;
}
