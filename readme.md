# COMP4411 - Modeler

## Quick Links
<ol>
	  <!--- li>Project #2 <a href=Project2_Demo_Schedule_draft.pdf>Demo</a>
	  <li><a href="COMP4411_PRO2_UPDATED.pptx">Slides 1</a>, <a href="Project2_HelpSession2.pptx"> Slides 2</a> <font size=-2>03/07/2022</font--->
          <li><a href="https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/ModSkelS20.zip">VS19 Skeleton + sample executable</a> -- it contains FLTK and should compile alone.
	</li><li> <a href="https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/modeler_checklist.txt">B&amp;W Checklist</a>
	  </li><li><a href="https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/sample_modeler_sp02.exe">Sample modeler</a> <b>(cool demo courtesy of Tai Pang Wu, need <a href="https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/plane.bmp">this texture</a> to run)</b>
          <!--- LI><A 
          href="../../tools/fltk_install.html">FLTK headers and library (zip)</A--> 
          </li><li><a href="https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/newdoc.html">How to use and extend the modeler</a> 
          </li><li><a href="https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/demos/">Sample modeler applications </a> <!--<li><a href="robotarm.cpp">Robotarm.cpp from Monday help session</a>--></li></ol>

## Basic Requirement

### Character & Control
 - [x] Constructs a character (D)
   - [x] can be composed solely of primitive shapes (box, generalized cylinder, sphere, and triangle)
   - [x] It should use at least ten primitives and at least four levels of hierarchy
   - [x] use at least one each of the `glTranslate()`, `glRotate()` and `glScale()` calls to position these primitives in space
   - [x] must also use `glPushMatrix()` and `glPopMatrix()` to nest your matrix transformations
 - [x] Adds at least one of these as a control knob (slider, actually) for some joint/component of your model (M)
 - [x] hobaseoking sliders to different features of your model using the functions for creating sliders provided by the skeleton (D)
 - [x] Some simple action as you scrub a slider back and forth (D)
 - [x] At least one of your controls must be tied to more than one joint/component
 - [x] This knob will change the input to some function that determines how several parts of your model can move
   - For example, in a model of a human, you might have a slider that straightens a leg by extending both the knee and hip joints.

### Camera
 - [x] implement your own version of gluLookAt(): `void Camera::lookAt(double *eye, double *at, double *up)` (M)
    - `double *eye`: it is **a 3 element arrays** that specify the position of the camera, the point it is looking at and the up vector in world co-ordinates. Using these arguments, your function should apply a series of translations and rotations (or construct a matrix) that effectively moves the camera to the specified position and orientation
    - You can use whatever method (i.e. the standard transformations (glTranslate, glRotate, glScale), matrix construction) as long as you are not using the gluLookAt itself
    - Originally it is implemented by using applyViewingTransform() uses gluLookAt() to perform a viewing transform

## Bonus (Bells and Whistles)

 - [x] 1W: Change the default light source to illuminate your scene more dramatically. (M)

 - [x] 1W: Allow for adjustable levels of detail for your character. You will need to add a UI control for this. (D)

 - [x] 1W: Come up with another whistle and implement it.  A whistle is something that extends the use of one of the things you are already doing.  It is part of the basic model construction, but extended or cloned and modified in an interesting way.  
    - By adjusting the number of cylinders in the tail

 - [x] 1B: Use a texture map on all or part of your character. (The safest way to do this is to implement your own primitives inside your model file that do texture mapping.) (M)
    - done for boxes only 

 - [x] 1B: Build a complex shape as a set of polygonal faces, using the "triangle" primitive to render them. (D)

 - [x] 1B: Make an additional "animated" sequence your character can perform.  Although you can try to use a timed callback "add_idle" (see [here](https://www.fltk.org/documentation.php) for more information), an easier solution is just to increment values each time your model's draw() function is called. If you use the menu option to turn on animation, your draw() function will be executed at around 30 times per second. (D)

 - [x] 1B: Add some widgets that control adjustable parameters to your model so that you can create individual-looking instances of your character.  Try to make these actually different individuals, not just "the red guy" and "the blue guy." (M)
    - But probably should come back and make nicer things??? like change the body to a cylinder for the fat cat...
    - Yes for sure

 - [x] 1B: Add a "Frame All" feature that would change the position of the camera such that your model is entirely visible in the model window, regardless of the pose and location of your model. (D)

 - [ ] 1B1W: The camera code has a constrained up vector -- modify the code to (1) handle camera twists (the framework is already there) and (2) to come up with an intuitive way for the user to twist the camera via mouse control.
    - demo: hold the right mouse and twist (kActionTwist probably)

 - [x] 2B: Add a function in your model file for drawing a new type of primitive. The following examples will definitely garner two bells ; if you come up with your own primitive, you will be awarded one or two bells based on its coolness. 1) Extruded surface - given two curves, create a surface by sweeping one curve along the path defined by the other curve. 2) Surfaces of rotation - given a curve and a rotation axis, sweep out a surface by rotating the curve around the axis. 3) Torus. (D)


 - [x] 2B: (Variable) Use some sort of procedural modeling (such as an [L-system](https://en.wikipedia.org/wiki/L-system)) to generate all or part of your character. Have parameters of the procedural modeler controllable by the user via control widgets. (D)


 - [ ] 2B: In addition to mood cycling, have your character react differently to UI controls depending on what mood they are in.  Again, there is some weight in this item because the character reactions are supposed to make sense in a story telling way.  Think about the mood that the character is in, think about the things that you might want the character to do, and then provide a means for expressing and controlling those actions.


 - [ ] 4B: One difficulty with hierarchical modeling using primitives is the difficulty of building "organic" shapes. It's difficult, for instance, to make a convincing looking human arm because you can't really show the bending of the skin and bulging of the muscle using cylinders and spheres. There has, however, been success in building organic shapes using [metaballs](https://en.wikipedia.org/wiki/Metaballs). Implement your hierarchical model and "skin" it with metaballs. Hint: look up "marching cubes" and "marching tetrahedra" --these are two commonly used algorithms for volume rendering. Here is a [demo application](https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/metaballs_demo.zip) by Joe Hall and Eugene Hsu that shows metaballs in action.


 - [ ] 8B: You might notice after building your model that it's difficult to have very "goal-oriented" motion. Given a model of a human, for instance, if the goal is to move the hand to a certain coordinate, we might have to change the shoulder angle, elbow angle -- maybe even the angle of the knees if the feet are constrained to one position. Implement a method, given a set of position constraints like:
   - left foot is at (1,0,2)
   - right foot is at (3,0,4)
   - left hand is at (7,8,2)

    that computes the intermediate angles necessary such that all constrains are satisfied (or, if the constraints can not be satisfied, the square of the distance violations is minimized).

 - [ ] 4B:  Make sure that all angle constraints are satisfied as well. In your model, for instance, you might have a slider that constrains the elbow angle between 30 and 180 degrees.  Look [here](https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/inverse-kinematics.pdf) for some related material.


 ### Bugs  
 - [x] When head width changes, the ear position does not follow
   - Now the width of the ear will scale up / down according to the width of the head