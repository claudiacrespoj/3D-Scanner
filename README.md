
# 3D-Scanner
3D Scanner built with C++ and openCV

This is a 3D Scanner built in C++ using **Open CV** library  and **OpenGL**



# Materials

**Mechanical Design** 	
To build the 3D scanner we used a wood disk with a radio of 150 mm to
place the object, there is an orifice in the middle of the disk where the axis of the motor will be placed 
![Disk](Pictures/Disk.png)

***Camera Hook***
This is where the camera will be placed with screws 
![ camera hook](Pictures/camerahook.png)

***Laser Hook***
Used to placed the laser
![Laser hook](Pictures/laserPlacement.png) 
	
***Slider***
This is build to slid the camera and the lase in the right angle 
![Slider](Pictures/slider.png)

***Ensemble***
How the ensemble piece looks like 
![Ensemble](Pictures/ensamble.png)
	
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

## Calibration

 Pattern utilized for calibration of the camera, each square is 5 mm x 5mm
 
 ![calibration](Pictures/calibratepicture.png)

**Skelotonized image of calibration**

![](Pictures/esqueletizadaprofile.jpg.png) 


Coordenates calculation
![img](http://www.sciweavers.org/tex2img.php?eq=%20X_%7Bd%7D%20%3D%20Px_%7Bd%7D%20-%20P_%7Bref%7D%0A&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

Transformation distance from pixel to millimeter 
![img](http://www.sciweavers.org/tex2img.php?eq=R%28X_%7Bd%7D%29%20%3D%20aX_%7B3%7D%5E%7Bd%7D%20%2B%20bX_%7B2%7D%5E%7Bd%7D%20%20%2B%20cX_%7Bd%7D%20%2Bd&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

Radio of profile 
![img](http://www.sciweavers.org/tex2img.php?eq=R_%7BR%7D%20%3D%20%5Cfrac%7BR%7D%7Bcos%2818.44%29%7D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

Conversion factor from pixels to milimeters (experimental)
![img](http://www.sciweavers.org/tex2img.php?eq=F_%7Bd%7D%20%3D%20%5Cfrac%7B10mm%7D%7BPY_%7Bd%7D%7D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)



conversion from pixel to mm
![img](http://www.sciweavers.org/tex2img.php?eq=F%28R_%7BR%7D%29%20%3D%20aR_%7B2%7D%5E%7BR%7D%20%2B%20bR_%7BR%7D%20%2B%20C&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

Height of profile
![img](http://www.sciweavers.org/tex2img.php?eq=A%3D%20PY_%7Bd%7D%20%2A%20F&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)
 
## Results

Image without processing
![](Pictures/result.jpg) 

Scan Result 
![](Pictures/scanresult.png)

Point plastic 
![](Pictures/pointplastic.png)

