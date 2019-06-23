#!/bin/bash

###################################################
#####    Vdovin Vladislav @vladislick 2019    #####
#####           CodeDraw installer            #####
###################################################

echo ""
echo -e "\e[1;42m WELCOME TO CODENDRAW INSTALLER \e[0m"
echo ""

### Массив строк, каждая из которых указывает пакеты-зависимости разных систем
declare -a dependencies
declare -a buildingDependencies

### Массив с названием пакетов, которые отсутствуют в системе
declare -a neeededPackages

### Прописываем все нужные пути для установки
iconsPath="/usr/share/icons/hicolor"
desktopPath="/usr/share/applications"
installPath="/usr/bin"

### Указываем параметры системы
system=(mint ubuntu debian manjaro arch fedora)
systemLogoColor=(42 43 41 42 46 44)

### СПИСКИ ЗАВИСИМОСТЕЙ
dependencies[0]="qt5-default libqt5serialport5 make gcc g++ "
dependencies[1]="qt5-default libqt5serialport5 make gcc g++ "
dependencies[2]="qt5-default libqt5serialport5 make gcc g++ "
dependencies[3]="qt5-base qt5-serialport make gcc "
dependencies[4]="qt5-base qt5-serialport make gcc "
dependencies[5]="qt5-devel make gcc-c++ gdb mesa-libGL-devel "

buildingDependencies[0]="qtbase5-private-dev libqt5serialport5-dev"
buildingDependencies[1]="qtbase5-private-dev libqt5serialport5-dev"
buildingDependencies[2]="qtbase5-private-dev libqt5serialport5-dev"
buildingDependencies[3]=""
buildingDependencies[4]=""
buildingDependencies[5]=""

for (( i = 0; i < ${#dependencies[@]}; i++ )); do
    dependencies[i]+=${buildingDependencies[i]}
done

### Список команд для проверки наличия пакета в разных системах
checkPackage=('dpkg -s ' 'dpkg -s ' 'dpkg -s ' 'pacman -Qi ' 'pacman -Qi ' 'which ')

### Список команд для установки пакета в разных системах
installPackages=('apt-get install ' 'apt-get install ' 'apt-get install ' 'pacman -S ' 'pacman -S ' 'yum install ')

### Список команд для удаления пакета в разных системах
removePackages=('apt-get remove ' 'apt-get remove ' 'apt-get remove ' 'pacman -R ' 'pacman -R ' 'yum remove ')

### Выводим название определившейся системы
echo -n "Your system: "
for i in ${!system[@]}; do
    if cat /usr/lib/*-release | grep ${system[$i]} &> /dev/null
    then
        ## Сохраняем индекс системы
        systemIndex=$i
        ## Получаем имя системы
        systemName=$(echo ${system[$systemIndex]} | sed  's/^\(.\)/\U\1/')
        ## Выводим название системы
        echo -ne "\e[1;${systemLogoColor[$systemIndex]}m $systemName Linux "
        
        if uname -a | grep "x86_64" &> /dev/null
        then
            echo -ne "64-bit"
        elif uname -a | grep "i686" &> /dev/null
        then
            echo -ne "32-bit"
        elif uname -a | grep "i386" &> /dev/null
        then 
            echo -ne "32-bit"
        else
            echo -ne $(uname -m)
        fi
        
        echo -e " \e[0m"
        break
    fi
done

echo ""
echo ""

### Проверяем зависимости
echo -e "\e[1;44m   CHECKING DEPENDENCIES \e[0m"
echo ""
needDependencies=0

### Берём отдельно каждый пакет
for (( packageIndex = 1; packageIndex <= $(echo ${dependencies[$systemIndex]} | wc -w); packageIndex++ )); do
    
    ## Хватаем имя одного из пакетов
    packageName=$(echo ${dependencies[$systemIndex]} | awk "{print \$$packageIndex;}")
    
    ## Проверяем его наличие в системе
    ${checkPackage[$systemIndex]} $packageName &> /dev/null
    if [ $? -eq 1 ]; then
        echo -e -n "\e[1;33mWarning: package '"
        echo -n $packageName
        echo -e "' were not found\e[0m"
        
        neededPackages+=$(echo "$packageName ")
        needDependencies=1
    fi
done

### Если все зависимости удовлетворены
if [ $needDependencies -eq 0 ]; then
    echo -e "\e[1;32mOK\e[0m"
    
### Если отсутствуют некоторые пакеты в системе
else
    echo ""
    echo -n "Would you like to install these automatically? (y/n) "
    read answer
    
    ## Пробуем установить пакеты
    if [ "$answer" = "y" ]; then
        echo "Trying to install this dependencies..."
        echo ""
        sudo ${installPackages[$systemIndex]} ${neededPackages[@]}
    fi
fi

echo "" 

### Компилируем проект 
echo -e "\e[1;46m    BUILDING THE PROJECT \e[0m"
echo ""

qmake CodenDraw.pro
make

echo ""

### Начинаем установку
echo -e "\e[1;45m           COPYING FILES \e[0m"
echo ""

#Копируем файлы
mkdir   ~/.config/CodenDraw
mkdir   ~/.config/CodenDraw/icons
mkdir   ~/.config/CodenDraw/icons/Papirus

cp -rfv icons/GitHubLogo.svg        ~/.config/CodenDraw/icons/
cp -rfv icons/QtLogo.svg            ~/.config/CodenDraw/icons/
cp -rfv icons/CodenDrawLogo.png     ~/.config/CodenDraw/icons/
cp -rf -R icons/Papirus            ~/.config/CodenDraw/icons/

sudo cp -rfv icons/24x24/codendraw.png   $iconsPath/24x24/apps
sudo cp -rfv icons/32x32/codendraw.png   $iconsPath/32x32/apps
sudo cp -rfv icons/64x64/codendraw.png   $iconsPath/64x64/apps
sudo cp -rfv icons/128x128/codendraw.png $iconsPath/128x128/apps

sudo cp -rfv CodenDraw $installPath/codendraw
sudo cp -rfv CodenDraw.desktop $desktopPath

### Даём права на выполнение и запись
sudo chmod a+x $installPath/codendraw

### Устанавливаем правила последовательного порта
sudo cp 99-platformio-udev.rules /etc/udev/rules.d/99-platformio-udev.rules
sudo udevadm control --reload-rules
sudo udevadm trigger

### Если Ubuntu, добавляем пользователя в нужные группы
if cat /usr/lib/*-release | grep ubuntu &> /dev/null 
then
    sudo usermod -a -G dialout $USER
    sudo usermod -a -G plugdev $USER
fi

### Обновляем базу данных иконок
update-desktop-database 
update-mime-database    ~/.local/share/mime/
gtk-update-icon-cache   $iconPath


echo ""

### Спрашиваем, хочет ли пользователь удалить файлы компиляции
echo -n "Would you like to remove building files and packages? (y/n) "
read answer2
if [ "$answer2" = "y" ]; then
    make clean
    rm Makefile
    rm .qmake*
    ${removePackages[$systemIndex]} ${buildingDependencies[$systemIndex]}
fi

### Выводим надпись окончания работы скрипта
echo ""
echo -e "\e[1;42m                    DONE \e[0m"
echo ""

exit 0
