# COMP4411 - Modeler
## Basic Requirements  
- We hooked the Rotate slider to the ears of the cat  

## Bonus (Bells and Whistles)

- W dramatic light: yes
  - You can set a new light position and increase the intensity.  
- W adjustable LoD: yes
  - You can change the detailness of the tails by changing the "Number of tail components"
  - Or "Number of Triangle in ear"
  - Or even changes the detailness of the torus by changing the model quality
- W your idea: yes
  - You can change the dimension of the model
  - So basically it is an extension of adjustable LoD
- B texture map: yes
  - Only implemented for the cat body, but it is good enough
- B polygonal faces (triangles): yes 
  - Please refer to the ear at the creature (cat) head
  - Again, you can control the number of triangles by dragging "Number of Triangle in ear"
- B animation: yes
  - A swinging tail, it's cute, isn't it?
  - Btw make sure set "Shows Inverse Kinematics Tail" to 0 to see our cute cute swinging tail
- B individual looking (not just red/green): yes
  - You can drag "Individual Instances" to see more
- B frame all: yes 
  - You will have to uncheck the Frame All function if you would like to use zoom  
- BW up vector: yes
  - Hold the ALT key and use the right mouse to twist the camera  
- 2B new primitives for each: yes
  - We did torus, which is opengl red book implementation
- 2B L-system: yes
  - A L System tree is attached as the second tail of the cat
  - You can use "L System Depth" to control the tree size
- 2B mood cycling: yes
  - "Mood Cycling" allows you to change the face of the cat
  - Also, the tail swinging speed varies according to the mood of the cat 
  - The light colour varies according to mood too  
- 4B metaballs: yes
  - Set "Enable Organic Model" to 1 for enabling this feature
  - It will alter the cat's head
- 8B goal-oriented motion: yes
  - Set "Shows Inverse Kinematics Tail" to 1 for enabling this feature
  - You can use Target X / Y / Z to control the position of the inverse kinematics tail
  - We choose to create another tail instead of reusing the same tail cuz we are lazy
- 4B joint contraints: yes
  - You can use "Angle Constraint for Inverse Kinematics" to control the angle contraints