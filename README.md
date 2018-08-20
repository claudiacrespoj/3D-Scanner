
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

After we calculate 
Coordenates calculation
>X_{d} = Px_{d} - P_{ref}

Transformation distance from pixel to millimeter 
>R(X_{d}) = aX_{3}^{d} + bX_{2}^{d}  + cX_{d} +d

Radio of profile 
>R_{R} = \frac{R}{cos(18.44)}

Conversion factor from pixel to mm
>F_{d} = \frac{10mm}{PY_{d}}

conversion from pixel to mm
>F(R_{R}) = aR_{2}^{R} + bR_{R} + C

Height of profile
A= PY_{d} * F
 
## Results
Image without processing

![](Pictures/result.jpg) 

Scan Result 
![](Pictures/scanresult.png)

Point plastic 
![](Pictures/pointplastic.png)



