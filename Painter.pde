class Painter {
  int xPos;
  int yPos;
  int alpha;
  int brushResolution; 
  int videoCounter;
  float brushSize;

  int numSides = 100;
  float theta = TWO_PI / numSides;

  Painter(int x, int y, int count, int _brushResolution, int _alpha, float _brushSize) {
    xPos = x;
    yPos = y;
    videoCounter = count;
    brushResolution = _brushResolution;
    alpha = _alpha; 
    brushSize = _brushSize;
  }

  void paint(PImage movie, int brushResolution, int alpha, float brushSize) {
    
    for (int i = 1; i <= brushResolution; i++) {
    pushMatrix();  
    translate(xPos, yPos);

    beginShape();
    tint(255, alpha/i);  
    texture(movie);
    for (int j=0; j<numSides+1; j++) {
      float x =  brushSize*sin(j * theta) * log(i+5) ;
      float y =  brushSize*cos(j * theta) * log(i+5) ;
      vertex(x, y, xPos + x, yPos + y);
    }
    endShape();
    popMatrix();
    }

  }
}

