#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();

    
    ofSetVerticalSync(true);
    andreas.load("/Users/andreasrefsgaard/Documents/Processing/VideoPainter/video_data_files/Andreas2s.mov");
    andreas.setLoopState(OF_LOOP_NORMAL);
    andreas.play();
    
    torben.load("/Users/andreasrefsgaard/Documents/Processing/VideoPainter/video_data_files/Torben2s.mov");
    torben.setLoopState(OF_LOOP_NORMAL);
    torben.play();
    
    brushImg.load("brush2.png");
    brushImg2.load("brush3.png");
    
    brushImg.setAnchorPercent(0.5, 0.5);
    brushImg2.setAnchorPercent(0.5, 0.5);
    
    int width = andreas.getWidth();
    int height = andreas.getHeight();
    
    maskFbo.allocate(width,height);
    fbo.allocate(width,height);
    
    maskFbo2.allocate(width,height);
    fbo2.allocate(width,height);
    
    // There are 3 of ways of loading a shader:
    //
    //  1 - Using just the name of the shader and ledding ofShader look for .frag and .vert: 
    //      Ex.: shader.load( "myShader");
    //
    //  2 - Giving the right file names for each one: 
    //      Ex.: shader.load( "myShader.vert","myShader.frag");
    //
    //  3 - And the third one it�s passing the shader programa on a single string;
    //


    #ifdef TARGET_OPENGLES
    shader.load("shaders_gles/alphamask.vert","shaders_gles/alphamask.frag");
    #else
    if(ofIsGLProgrammableRenderer()){
    	string vertex = "#version 150\n\
    	\n\
		uniform mat4 projectionMatrix;\n\
		uniform mat4 modelViewMatrix;\n\
    	uniform mat4 modelViewProjectionMatrix;\n\
    	\n\
    	\n\
    	in vec4  position;\n\
    	in vec2  texcoord;\n\
    	\n\
    	out vec2 texCoordVarying;\n\
    	\n\
    	void main()\n\
    	{\n\
	        texCoordVarying = texcoord;\
    		gl_Position = modelViewProjectionMatrix * position;\n\
    	}";
		string fragment = "#version 150\n\
		\n\
		uniform sampler2DRect tex0;\
		uniform sampler2DRect maskTex;\
        in vec2 texCoordVarying;\n\
		\
        out vec4 fragColor;\n\
		void main (void){\
		vec2 pos = texCoordVarying;\
		\
		vec3 src = texture(tex0, pos).rgb;\
		float mask = texture(maskTex, pos).r;\
		\
		fragColor = vec4( src , mask);\
		}";
		shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
		shader.bindDefaults();
		shader.linkProgram();
    }else{
		string shaderProgram = "#version 120\n \
		#extension GL_ARB_texture_rectangle : enable\n \
		\
		uniform sampler2DRect tex0;\
		uniform sampler2DRect maskTex;\
		\
		void main (void){\
		vec2 pos = gl_TexCoord[0].st;\
		\
		vec3 src = texture2DRect(tex0, pos).rgb;\
		float mask = texture2DRect(maskTex, pos).r;\
		\
		gl_FragColor = vec4( src , mask);\
		}";
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
		shader.linkProgram();
    }
    #endif
    
    
    
    

    // Lets clear the FBOs
    // otherwise it will bring some junk with it from the memory    
    
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    
    
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    
    maskFbo2.begin();
    ofClear(0,0,0,255);
    maskFbo2.end();
    
    
    fbo2.begin();
    ofClear(0,0,0,255);
    fbo2.end();
    
    
    bBrushDown = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    andreas.update();
    torben.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    ofEnableAlphaBlending();
    //----------------------------------------------------------
    // this is our alpha mask which we draw into.
    
    //delete mode
if (bBrushDown && eraserBrush) {
    
    maskFbo2.begin();
    brushImg2.draw(mouseX,mouseY,150,150);
    maskFbo.end();
    
    maskFbo.begin();
    brushImg2.draw(mouseX,mouseY,150,150);
    maskFbo2.end();
    
    }
    
    
    if(bBrushDown && layer == 0 && eraserBrush==false) {
        //Clear the other layer where your are drawing
        maskFbo2.begin();
        brushImg2.draw(mouseX,mouseY,150,150);
        maskFbo2.end();
        
        maskFbo.begin();
        brushImg.draw(mouseX,mouseY,150,150);
        maskFbo.end();
        
    }
    
    //----------------------------------------------------------
    // HERE the shader-masking happends
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    fbo.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent by default
    ofClear(0, 0, 0, 0);
    
    shader.begin();
    // here is where the fbo is passed to the shader
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
    
    torben.draw(0, 0);
    
    shader.end();
    fbo.end();
    ofEnableAlphaBlending();
    //----------------------------------------------------------
    // FIRST draw the background image
    //andreas.draw(0,0);
    
    // THEN draw the masked fbo on top
    fbo.draw(0,0);
    
    
    //Layer2
    if(bBrushDown == 1 && layer ==1 && eraserBrush==false) {
        
        //Clear the other layer where your are drawing
        maskFbo.begin();
        brushImg2.draw(mouseX,mouseY,150,150);
        maskFbo.end();
        
        
        maskFbo2.begin();
        brushImg.draw(mouseX,mouseY,150,150);
        maskFbo2.end();
    }
    
    //----------------------------------------------------------
    // HERE the shader-masking happends
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    fbo2.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent by default
    ofClear(0, 0, 0, 0);
    
    shader.begin();
    // here is where the fbo is passed to the shader
    shader.setUniformTexture("maskTex", maskFbo2.getTextureReference(), 1 );
    
    andreas.draw(0, 0);

    shader.end();
    fbo2.end();
    ofEnableAlphaBlending();
    //----------------------------------------------------------
    // FIRST draw the background image
    //andreas.draw(0,0);

    
    // THEN draw the masked fbo on top
    fbo2.draw(0,0);
    
    
    
    //----------------------------------------------------------
    ofDrawBitmapString("Drag the Mouse to draw", 15,15);
    ofDrawBitmapString("Press 'c' to clear layer0 and 'd' to clear layer1", 15, 30);
    ofDrawBitmapString("Press 'e' to toggle eraser" + ofToString(eraserBrush), 15, 45);
    ofDrawBitmapString("Current layer: " + ofToString(layer), 15, 60);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'c'){
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    } else if (key == 'd') {
    maskFbo2.begin();
    ofClear(0,0,0,255);
    maskFbo2.end();
    } else if (key == '0') {
        layer = 0;
    } else if (key == '1') {
        layer = 1;
    } else if (key == 'e') {
        eraserBrush =! eraserBrush;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bBrushDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bBrushDown = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
