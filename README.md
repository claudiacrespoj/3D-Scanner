
# 3D-Scanner
3D Scanner built with C++ and openCV

This is a 3D Scanner built in C++ using **Open CV** library  and **OpenGL**



# Materials

**Mechanical Design** 	
	To build the 3D scanner we used a wood disk with a radio of 150 mm to 		  place the object, there is an orifice in the middle of the disk where the axis of the motor will be placed 
		![Disk](3D-Scanner/Pictures/Disk.png)
		***Camera Hook***
This is where the camera will be placed with screws 
![ camera hook](3D-Scanner/Pictures/camerahook.png)
	***Laser Hook***
	Used to placed the laser
	![Laser hook](3D-Scanner/Pictures/laserPlacement.png) 
	***Slider***
This is build to slid the camera and the lase in the right angle 
![Slider](3D-Scanner/Pictures/slider.png)
***Ensemble***
	How the ensemble piece looks like 
	![Ensemble](3D-Scanner/Pictures/ensamble.png)
	
**Electronic Design**

***5v and 12v Power source***
We need a 5v source to power the LEDs and 12v for the stepper motor circuit
***Driver for stepper motor***
We used the L283D
**Relay circuit**
We used relay circuit to turn on/off the LEDs to take color pictures of the elements we were scanning

**A bit of theory **

For the 3D scanner we used the triangulation method if you know the distance between the camera and laser 
![triangulation method](https://www.researchgate.net/profile/Gabriel_Taubin/publication/266660058/figure/fig1/AS:614348510748672@1523483507061/Active-methods-for-3D-scanning-Left-Conceptual-diagram-of-a-3D-slit-scanner_Q320.jpg)

Once we get all the points we used Bezier Surface to reconstruct the element we wanted to scan
![](https://upload.wikimedia.org/wikipedia/commons/e/ea/NURBS_3-D_surface.gif)
 
## Results
