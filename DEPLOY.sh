cd
	#COMMONS
	
	if test -d /home/utnso/so-commons-library; then
		cd /home/utnso/so-commons-library	
		sudo make unistall
		cd ..
		sudo rm -r /so-commons-library
	fi

cd
	cd /home/utnso
	git clone https://github.com/sisoputnfrba/so-commons-library.git
	cd so-commons-library
	sudo make install

cd

	cd /home/utnso/tp-2020-1c-Fritsplank/Library/Library/Debug
	make clean
	make all

cd

	cd /home/utnso/tp-2020-1c-Fritsplank/Broker/Debug
	make clean
	make all

cd

	cd /home/utnso/tp-2020-1c-Fritsplank/GameCard/Debug
	make clean
	make all

cd

	cd /home/utnso/tp-2020-1c-Fritsplank/GameBoy/Debug
	make clean
	make all

cd

	cd /home/utnso/tp-2020-1c-Fritsplank/Team/Debug
	make clean
	make all

cd