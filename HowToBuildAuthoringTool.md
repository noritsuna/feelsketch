# Compile and starting up #

## install Eclipse ##

Install Eclipse IDE for Java Developers

http://www.eclipse.org/downloads/moreinfo/java.php



## Setup Maven2 ##

Install Maven2 and set the path through, so it can be used.

http://maven.apache.org/


## Registering the library ##

for Windows : release/install.bat

for Mac/Linux: : release/install.sh


## Registering Maven2 repository ##

Start up the Eclipse, open the menu like shown below, and then register repository for Maven2.

Window->Preferences->Java->Build Path->Classpath Variables

Then setup M2\_REPO [home directory](your.md)/.m2/repository


## Register “Feel Sketch” authoring tool project ##

File->Import->General->Existing Projects into Workspace->Select root directory

Then appoint the file where the extracted (defrosted) “Feel Sketch” authoring tool will be put.
Push the ‘Finish’ button. Then everything is entered.


## Setting up the Run ##

Run->Run Configurations->Java Application

Right click, select ‘new.’

Insert (type in):
‘Feel Sketch authoring tool’ in Name
‘feelsketch’ in Project
‘com.nullfish.app.feel\_sketch.Launcher’ in Main Class
then push ‘Apply” button.


## Start up ##

By selecting ‘Feel Sketch authoring tool’ using Run, the application will start up.





# How to create an action (practice) file (create Runnable JAR) #

## Start up the Runnable JAR menu ##

File->Export->Java->Runnable JAR file




## Create Runnable JAR ##

A ‘Runnable JAR File Export’ screen will start up.
Select and type in:
‘Feel Sketch authoring tool’ in Launch Configuration
‘feelsketchAuthoring.jar’ in Export destination
‘Package required libraries into generated JAR’ in Livrary handling

Push ‘Finish’ button to complete everything.