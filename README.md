# Balancing an Inverted Pendulum on a Cart: Capstone F2022

This repository holds all code I worked on for my CS capstone. The goal of the capstone was to balance an inverted pendulum on a cart utilizing an Elegoo Uno R3.
Prior to this project, I had no experience with any sort of hardware and arduino so this was a learning process. In this README, I will detail the contents of each file in the order they were worked on,

### 1. blink.ino
This is the "Hello world!" equivalent to the arduino. Since this was my first time working with it, I thought it was necessary to start with this as well as document it.
  
### 2. /left_right
Left right was my introduction to the stepper motor as well as the unit itself. In this, I learned how the stepper motor works, and how exactly the cart was affected by this code.
### 3. /accel_demo
Within this file, I experimented with using the AccelStepper library which allows me to control the motor using acceleration. Also in this library, I would know the exact location of the motor without having to keep count - this would help later in the implementation of the PID controller. It was later updated to display a variety of accelerations during the fair.
### 4. /demo
Demo is not actually the demonstration for this project. Instead, this .ino file is where I learned how to calculate the angle of the rotary encoder. Understanding the encoder was vital to creating a correct implementation. The `serial.println(i)` does make the readings inaccurate, but this was addressed in any future implementation. Again, this file strictly reads the encoder value and calculates the angle.
### 5. /swing_up
With the acceleration under my belt and the angle being calculated, the next step is the swing up. This file holds the implementation of getting the pendulum to swing-up to a state where balancing can occur. This is done by having the knowledge of when $\dot{\theta}=0$ and which direction the swing is coming in from. With this knowledge, the cart waits until the angle reaches 0 and moves with the direction of the swing to one of the bounds.
### 6. /balance
Balance holds the balancing code, which primarily adds the PID and all needed constants and variables. The PID takes the squared error of the angle and attempts to find how many steps, and in which direction, the cart must move to balance the pendulum. This implementation mostly oscillates around $\theta=0$ and eventually falls. With this being said, the hardware - as implemented - cannot do both the swing-up and the balance as it begins to miss encoder readings. A second kernel or interrupts may have solved this.
### 7. /balanceNorm
This is essentially the same implementation as balance but the constants for the PID are a lot more "normal" with values near 1. This also only takes the last sample of the integral and the derivative instead of the 20 in the balance implementaion. Since we can't swing up and balance, the further samples don't truly matter as the pendulum won't have any crazy acceleration. Also, with the 20 samples, there is no set rate of reading the encoder so the values are not very accurate.
