float brushSize = 1;
int brushResolution;
int alpha;

ArrayList<Painter> painters;
import controlP5.*;
ControlP5 cp5;

import processing.video.*;

Movie movie;
Movie movie2;

int myColor = color(0, 0, 0);
color backgroundC = #dfdfdf;

int counter = 0;

void setup() {
  size(1440, 810, OPENGL);
  smooth();
  noStroke();
  textureMode(IMAGE);

  // Load and play the video in a loop
  movie = new Movie(this, "Andreas2s.mov");
  movie.loop();
  movie.volume(0);

  movie2 = new Movie(this, "Torben2s.mov");
  movie2.loop();
  movie2.volume(0);

  painters = new ArrayList<Painter>();

  cp5 = new ControlP5(this);

  // add a vertical slider
  cp5.addSlider("alphaVal")
    .setPosition(10, 100)
      .setSize(200, 20)
        .setRange(0, 255)
          .setValue(50)
            ;


  // add a vertical slider
  cp5.addSlider("brushResolution")
    .setPosition(10, 150)
      .setSize(200, 20)
        .setRange(1, 5)
          .setValue(1)
            ;

  // add a vertical slider
  cp5.addSlider("brushSize")
    .setPosition(10, 200)
      .setSize(200, 20)
        .setRange(1, 75)
          .setValue(30)
            ;
}


void movieEvent(Movie m) {
  m.read();
}

void draw() {
  background(backgroundC);
  float normMX=map(mouseX, 0, width, 0, 1);
  float normMY=map(mouseY, 0, height, 0, 1);
  for (int i = 0; i < painters.size (); i++) {
    Painter p = painters.get(i);
    if (p.videoCounter == 0) {
      p.paint(movie, p.brushResolution, p.alpha, p.brushSize);
    } else if (p.videoCounter == 1) {
      p.paint(movie2, p.brushResolution, p.alpha, p.brushSize);
    }
  }
  fill(0);
  text("Now painting with layer: " + counter + ", press a key to switch", 10, 40);
  text("Press d to clear screen", 10, 60);
}

void mouseDragged() {
  painters.add(new Painter(mouseX, mouseY, counter, brushResolution, alpha, brushSize));
}

//void keyPressed() {
//  counter++;
//  if (counter>1) {
//    counter= 0;
//  }
//}

void alphaVal(int _alphaVal) {
  alpha = _alphaVal;
}


void brushResolution(int _brushResolution) {
  brushResolution = _brushResolution;
}

void brushSize(int _brushSize) {
  brushSize = _brushSize;
}

void keyPressed() {
  if (key == 'd') {
      for (int i = painters.size() - 1; i >= 0; i--) {
  //Painter p = painters.get(i);
    painters.remove(i);
}

//ALTERNATIV - Only remove the first
//    for (int i = 0; i < painters.size (); i++) {
//      //Painter p = painters.get(i);
//      painters.remove(0);
//       }  
    
  } else {
    
    counter++;
      if (counter>1) {
      counter= 0;
      }
    
  }
}


  


