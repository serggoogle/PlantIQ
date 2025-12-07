FROM ubuntu:22.04
RUN apt-get update && apt-get install -y locales curl udev python3.10-venv zip unzip git vim

RUN <<EOF
	sed -i '/force_color_prompt=yes/s/^#//' ~/.bashrc
	curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
	curl -o 99-platformio-udev.rules -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules
	python3 get-platformio.py && rm get-platformio.py
	mv 99-platformio-udev.rules /etc/udev/rules.d/99-platformio-udev.rules
	mkdir -p /usr/local/bin && \
		ln -s ~/.platformio/penv/bin/platformio /usr/local/bin/platformio && \
		ln -s ~/.platformio/penv/bin/pio /usr/local/bin/pio && \
		ln -s ~/.platformio/penv/bin/piodebuggdb /usr/local/bin/piodebuggdb
	mkdir /opt/plant-monitoring-platform
EOF


SHELL ["/bin/bash", "-c"]
RUN curl -s "https://get.sdkman.io" | bash
RUN source "$HOME/.sdkman/bin/sdkman-init.sh" && \
	sdk install java 17.0.11-tem && \
	sdk install maven 3.8.6

WORKDIR /opt/plant-monitoring-platform
CMD ["tail", "-f", "/dev/null"]