#!/bin/bash

### Прописываем все нужные пути для установки
iconsPath="/usr/share/icons/hicolor"
desktopPath="/usr/share/applications"
installPath="/usr/bin"

sudo rm -v $installPath/codendraw
sudo rm -v $desktopPath/CodenDraw.desktop

sudo rm -v $iconsPath/24x24/apps/codendraw.png
sudo rm -v $iconsPath/32x32/apps/codendraw.png
sudo rm -v $iconsPath/64x64/apps/codendraw.png
sudo rm -v $iconsPath/128x128/apps/codendraw.png

rm -v -R ~/.config/CodenDraw

### Обновляем базу данных иконок
update-desktop-database 
update-mime-database    ~/.local/share/mime/
gtk-update-icon-cache   $iconPath
