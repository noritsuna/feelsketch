# Preparing for the environment

## Eclipse
Preparing for the environment

http://www.eclipse.org/downloads/

## Android SDK and NDK
Install Android SDK.

http://developer.android.com/sdk/index.html

Install ADT Plug in for Eclipse.

http://developer.android.com/sdk/eclipse-adt.html

Add Android 1.6 Platform.

http://developer.android.com/sdk/adding-components.html

Install Android 1.6 NDK

http://developer.android.com/sdk/ndk/1.6_r1/index.html


# Build ‘Feel Sketch Browser’

## Downloading the source
```
cd <ndk>/apps
git clone https://github.com/noritsuna/feelsketch.git
```

## Build the native code
Build the native code’s common (mutual) library with NDK.
```
cd <ndk>
make APP=FeelSketchBrowser
```

## Performe
Create ‘project’ by ‘Eclipse.’

File->New->Android Project

Select ‘Android Project.’ Check (select) the ‘Create project from existing source’ inside the ‘Contents.’ Type in Location,

`<ndk>`/apps/FeelSketchBrowser/project

Then click ‘Finish’ button.


When the project is created、

Project->Clean...

Select ‘Clear.’

Run->Android Application

Start perform.