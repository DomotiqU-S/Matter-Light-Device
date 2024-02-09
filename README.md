# Matter Light Device

**Content**
- [Matter Light Device](#matter-light-device)
    - [Installation](#installation)
    - [Device type](#device-type)

### Installation

This program is a bit different from normal IDF program. In order to compile the program you need to install the matter framework from ESP-IDF. You can find the instructions and files here [ESP-Matter](https://github.com/espressif/esp-matter).
Once the installation is completed, you can source the IDF path with:

```
source $HOME/esp-idf/export.sh
```
Then you need to source the matter path in the root folder of the program:

```
source $HOME/esp-matter/export.sh
```
You should be setup to compile the program.

### Device type

The default light type is dimmable light. You can change the type on line 47 in the file [app_main](./main/app_main.cpp).
There are four types of light:
| Type   |      Variable type      |
|----------|:-------------:|
| on off light |  on_off_light |
| dimmable light |    dimmable_light   |
| color temperature light | color_temperature_light |
| RBG color | extended_color_light |