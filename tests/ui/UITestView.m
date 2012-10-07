/*
 * iSGL3D: http://isgl3d.com
 *
 * Copyright (c) 2010-2012 Stuart Caunt
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#import "UITestView.h"


@interface Simple3DView ()
@end


#pragma mark -
@implementation UITestView

- (id)init {
	
	if (self = [super init]) {
		// Create a button to calibrate the accelerometer
		Isgl3dTextureMaterial * calibrateButtonMaterial = [Isgl3dTextureMaterial materialWithTextureFile:@"angle.png" shininess:0.9 precision:Isgl3dTexturePrecisionMedium repeatX:NO repeatY:NO];
		Isgl3dGLUIButton * calibrateButton = [Isgl3dGLUIButton buttonWithMaterial:calibrateButtonMaterial];
		[self.scene addChild:calibrateButton];
		[calibrateButton setX:8 andY:264];
		[calibrateButton addEvent3DListener:self method:@selector(calibrateButtonPressed:) forEventType:TOUCH_EVENT];
	
		// Create a button to pause the scene
		Isgl3dTextureMaterial * pauseButtonMaterial = [Isgl3dTextureMaterial materialWithTextureFile:@"pause.png" shininess:0.9 precision:Isgl3dTexturePrecisionMedium repeatX:NO repeatY:NO];
		Isgl3dGLUIButton * pauseButton = [Isgl3dGLUIButton buttonWithMaterial:pauseButtonMaterial];
		[self.scene addChild:pauseButton];
		[pauseButton setX:424 andY:264];
		[pauseButton addEvent3DListener:self method:@selector(pauseButtonPressed:) forEventType:TOUCH_EVENT];
	
		// Create a button to allow movement of the camera
		Isgl3dTextureMaterial * cameraButtonMaterial = [Isgl3dTextureMaterial materialWithTextureFile:@"camera.png" shininess:0.9 precision:Isgl3dTexturePrecisionMedium repeatX:NO repeatY:NO];
		Isgl3dGLUIButton * cameraButton = [Isgl3dGLUIButton buttonWithMaterial:cameraButtonMaterial];
		[self.scene addChild:cameraButton];
		[cameraButton setX:8 andY:8];
		[cameraButton addEvent3DListener:self method:@selector(cameraButtonPressed:) forEventType:TOUCH_EVENT];
        
        // Create a number of progress bars to test each variation.
        
        progress = 0.0;
        Isgl3dTextureMaterial* progressMaterial = [Isgl3dTextureMaterial materialWithTextureFile:@"tex.png" shininess:0.0 precision:Isgl3dTexturePrecisionMedium repeatX:NO repeatY:NO];

        horizontalRegularProgress = [Isgl3dGLUIProgressBar progressBarWithMaterial:progressMaterial width:64 height:64 vertical:NO];
        [horizontalRegularProgress setX:64 andY:8];
        horizontalRegularProgress.progress = progress;
        [self.scene addChild:horizontalRegularProgress];

        horizontalSwappedProgress = [Isgl3dGLUIProgressBar progressBarWithMaterial:progressMaterial width:64 height:64 vertical:NO];
        horizontalSwappedProgress.isSwapped = YES;
        [horizontalSwappedProgress setX:128 andY:8];
        horizontalSwappedProgress.progress = progress;
        [self.scene addChild:horizontalSwappedProgress];

        verticalRegularProgress = [Isgl3dGLUIProgressBar progressBarWithMaterial:progressMaterial width:64 height:64 vertical:YES];
        [verticalRegularProgress setX:192 andY:8];
        verticalRegularProgress.progress = progress;
        [self.scene addChild:verticalRegularProgress];
        
        verticalSwappedProgress = [Isgl3dGLUIProgressBar progressBarWithMaterial:progressMaterial width:64 height:64 vertical:YES];
        verticalSwappedProgress.isSwapped = YES;
        [verticalSwappedProgress setX:256 andY:8];
        verticalSwappedProgress.progress = progress;
        [self.scene addChild:verticalSwappedProgress];

        [self schedule:@selector(tick:)];
	}
	
	return self;
}

- (void) dealloc {

	[super dealloc];
}

- (void) calibrateButtonPressed:(Isgl3dEvent3D *)event {
	NSLog(@"Calibrate button pressed");
	[[Isgl3dDirector sharedInstance] resume];
}

- (void) pauseButtonPressed:(Isgl3dEvent3D *)event {
	NSLog(@"Pause button pressed");
	[[Isgl3dDirector sharedInstance] pause];
}

- (void) cameraButtonPressed:(Isgl3dEvent3D *)event {
	NSLog(@"Camera button pressed");
}

- (void) tick:(float)dt {
    progress = (progress >= 100.0 ? 0.0 : progress + 1.0);
    horizontalRegularProgress.progress = progress;
    horizontalSwappedProgress.progress = progress;
    verticalRegularProgress.progress = progress;
    verticalSwappedProgress.progress = progress;
}

@end

#pragma mark UIBackgroundView


@implementation UIBackgroundView

- (id)init {
	
	if (self = [super init]) {
		// Create a button to calibrate the accelerometer
		Isgl3dTextureMaterial *backgroundMaterial = [Isgl3dTextureMaterial materialWithTextureFile:@"ui_bg.pvr" shininess:0 precision:Isgl3dTexturePrecisionMedium repeatX:NO repeatY:NO];
		Isgl3dGLUIImage *background = [Isgl3dGLUIImage imageWithMaterial:backgroundMaterial andRectangle:self.viewport width:self.viewport.size.width height:self.viewport.size.height];
		[self.scene addChild:background];
	}
	return self;
}

- (void) dealloc {

	[super dealloc];
}


@end

#pragma mark Simple3DView

#import "Isgl3dDemoCameraController.h"

@implementation Simple3DView 

- (id)init {
	
	if (self = [super init]) {
		// Create the primitive
		Isgl3dTextureMaterial * material = [Isgl3dTextureMaterial materialWithTextureFile:@"red_checker.png" shininess:0.9 precision:Isgl3dTexturePrecisionMedium repeatX:NO repeatY:NO];
		Isgl3dTorus * torusMesh = [Isgl3dTorus meshWithGeometry:2 tubeRadius:1 ns:32 nt:32];
		_torus = [self.scene createNodeWithMesh:torusMesh andMaterial:material];
	
		// Add light
		Isgl3dLight * light  = [Isgl3dLight lightWithHexColor:@"FFFFFF" diffuseColor:@"FFFFFF" specularColor:@"FFFFFF" attenuation:0.005];
		light.position = Isgl3dVector3Make(5, 15, 15);
		[self.scene addChild:light];

		// Create camera controller
        Isgl3dNodeCamera *nodeCamera = (Isgl3dNodeCamera *)self.defaultCamera;
        
		_cameraController = [[Isgl3dDemoCameraController alloc] initWithNodeCamera:nodeCamera andView:self];
		_cameraController.orbit = 14;
		_cameraController.theta = 30;
		_cameraController.phi = 30;

		// Schedule updates
		[self schedule:@selector(tick:)];
	}
	return self;
}

- (void) dealloc {
	[_cameraController release];
    _cameraController = nil;
		
	[super dealloc];
}

- (void) tick:(float)dt {
	[_torus rotate:0.5 x:0 y:1 z:0];
	
	[_cameraController update];
}

- (void) onActivated {
	// Add camera controller to touch-screen manager
	[[Isgl3dTouchScreen sharedInstance] addResponder:_cameraController withView:self];
}

- (void) onDeactivated {
	// Remove camera controller from touch-screen manager
	[[Isgl3dTouchScreen sharedInstance] removeResponder:_cameraController];
}

@end



#pragma mark AppDelegate

/*
 * Implement principal class: simply override the createViews method to return the desired demo view.
 */
@implementation AppDelegate

- (void) createViews {
	// Create 2D view background and add to Isgl3dDirector
	Isgl3dView *background = [UIBackgroundView view];
    background.displayFPS = YES;
	[[Isgl3dDirector sharedInstance] addView:background];

	// Create 3D view and add to Isgl3dDirector
	Isgl3dView *view = [Simple3DView view];
	[[Isgl3dDirector sharedInstance] addView:view];

	// Create UI and add to Isgl3dDirector
	Isgl3dView *ui = [UITestView view];
	[[Isgl3dDirector sharedInstance] addView:ui];

}

@end
