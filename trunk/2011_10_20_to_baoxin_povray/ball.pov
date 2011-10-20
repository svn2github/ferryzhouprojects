#version 3.6;
global_settings {  assumed_gamma 1.0 }
//---------------------------------------
camera{ ultra_wide_angle
        angle 75
        right x*image_width/image_height
        location  <0.0 , 1.0 ,-3.0>          // to generate another view, change 0.0 to 0.2 or whatever
        look_at   <0.0 , 0.0 , 0.0> }        // also change 0.0 to 0.2 in this line if you want parallel principle axes
//---------------------------------------
light_source{ <1500,2500,-2500>
              color rgb<1,1,1> }
//---------------------------------------
sky_sphere{ pigment{color rgb<1,1,1>}}
//---------------------------------------
// the rotating sphere:
sphere{ <0,0,0>, 0.5
        texture{ pigment{ color rgb<1, 1,1>}
                 normal{ leopard -5 scale 0.015}
                 finish { diffuse 0.9 phong 0.1}
               } // end of texture 1

        texture{ pigment{
                 image_map{ gif "POV-Ray_215.gif"
                            map_type 0
                            once
                            transmit 215, 1
                          }
                 translate<-0.5,-0.5,0>
                 scale<0.75,1,1>*0.85
                 } // end pigment
                 normal{ leopard -5 scale 0.015}
               } // end of texture 2
        rotate < 0,360*clock 0>//  <-!!!!
       } // end of sphere ---------------
//----------------------------------- end