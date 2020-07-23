if test -f Broker/Debug/ArchivoDump.bin; then
	rm Broker/Debug/ArchivoDump.bin
fi

export LD_LIBRARY_PATH=/home/utnso/tp-2020-1c-Fritsplank/Library/Library/Debug
cd Broker/Debug
./Broker