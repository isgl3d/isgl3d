/*
 * iSGL3D: http://isgl3d.com
 *
 * Copyright (c) 2010-2011 Stuart Caunt
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

#import "CameraZoomAndFocusDemoView.h"

@implementation CameraZoomAndFocusDemoView

- (void) dealloc {

	[super dealloc];
}

- (void) initView {
	[super initView];

	[_camera setTranslation:0 y:0 z:1000];
		
	self.isLandscape = YES;
	
	_camera.focus = 4;
	_camera.zoom = 10;
	
	_zoom = _camera.zoom;
	_focus = _camera.focus;
	
	[[Isgl3dTouchScreen sharedInstance] addResponder:self];
	_theta = 0;	 	
	_moving = NO;	
}

- (void) initScene {
	[super initScene];
	
	Isgl3dColorMaterial * redMaterial = [[Isgl3dColorMaterial alloc] initWithHexColors:@"FF0000" diffuse:@"FF0000" specular:@"FF0000" shininess:0];
	Isgl3dColorMaterial * greenMaterial = [[Isgl3dColorMaterial alloc] initWithHexColors:@"00FF00" diffuse:@"00FF00" specular:@"00FF00" shininess:0];
	Isgl3dColorMaterial * blueMaterial = [[Isgl3dColorMaterial alloc] initWithHexColors:@"0000FF" diffuse:@"0000FF" specular:@"0000FF" shininess:0];
	Isgl3dColorMaterial * yellowMaterial = [[Isgl3dColorMaterial alloc] initWithHexColors:@"FFFF00" diffuse:@"FFFF00" specular:@"FFFF00" shininess:0];
	
	Isgl3dPlane * sidePlane = [[Isgl3dPlane alloc] initWithGeometry:150 height:350 nx:10 ny:10];
	Isgl3dPlane * horizontalPlane = [[Isgl3dPlane alloc] initWithGeometry:850 height:150 nx:10 ny:10];
	
	for (int i = 0; i < 3; i++) {

		Isgl3dMeshNode * bottomNode = [_scene createNodeWithMesh:horizontalPlane andMaterial:blueMaterial];
		[bottomNode rotate:-90 x:1 y:0 z:0];
		[bottomNode translate:0 y:-200 z:-(i - 1) * 1000];
		bottomNode.doubleSided = YES;

		Isgl3dMeshNode * topNode = [_scene createNodeWithMesh:horizontalPlane andMaterial:greenMaterial];
		[topNode rotate:90 x:1 y:0 z:0];
		[topNode translate:0 y:200 z:-(i - 1) * 1000];
		topNode.doubleSided = YES;

		Isgl3dMeshNode * rightNode = [_scene createNodeWithMesh:sidePlane andMaterial:redMaterial];
		[rightNode rotate:-90 x:0 y:1 z:0];
		[rightNode translate:450 y:0 z:-(i - 1) * 1000];
		rightNode.doubleSided = YES;
		
		Isgl3dMeshNode * leftNode = [_scene createNodeWithMesh:sidePlane andMaterial:yellowMaterial];
		[leftNode rotate:90 x:0 y:1 z:0];
		[leftNode translate:-450 y:0 z:-(i - 1) * 1000];
		leftNode.doubleSided = YES;
	}
	
	[redMaterial release];
	[greenMaterial release];
	[blueMaterial release];
	[yellowMaterial release];
	
	[sidePlane release];
	[horizontalPlane release];
	
	// Add shadow casting light
	Isgl3dLight * light  = [[Isgl3dLight alloc] initWithHexColor:@"FFFFFF" diffuseColor:@"FFFFFF" specularColor:@"FFFFFF" attenuation:0];
	[light setTranslation:400 y:1000 z:400];
	[_scene addChild:light];

	// Initialise accelerometer
	[[Isgl3dAccelerometer sharedInstance] setup:30];
}

- (void) updateScene {
	[super updateScene];
	
	float orbitDistance = 1000;
	float y = orbitDistance * cos([[Isgl3dAccelerometer sharedInstance] tiltAngle]);
	float radius = orbitDistance * sin([[Isgl3dAccelerometer sharedInstance] tiltAngle]);

	_theta += 0.05 * [[Isgl3dAccelerometer sharedInstance] rotationAngle];
	float x = radius * sin(_theta);
	float z = radius * cos(_theta);
	[_camera setTranslation:x y:y z:z];

	if (_moving) {
		_focus += _dFocus;
		_zoom += _dZoom;
		
		if (_zoom < 0.1) {
			_zoom = 0.1;
		}
		if (_focus < 1) {
			_focus = 1;
		}
		
		_camera.focus = _focus;
		_camera.zoom = _zoom;
		
		_moving = NO;
	}

//	NSLog(@"Angle = %f", [[Accelerometer sharedInstance] rotationAngle] * 180 / M_PI);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	NSEnumerator * enumerator = [touches objectEnumerator];
	UITouch * touch = [enumerator nextObject];

	CGPoint	location = [touch locationInView:self];
    _lastTouchX = location.x;
    _lastTouchY = location.y;
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	_moving = NO;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	NSEnumerator * enumerator = [touches objectEnumerator];
	UITouch * touch = [enumerator nextObject];

	CGPoint	location = [touch locationInView:self];

	_dZoom = (location.x - _lastTouchX) / 20;
	_dFocus = (location.y - _lastTouchY) / 10;

    _lastTouchX = location.x;
    _lastTouchY = location.y;

	_moving = YES;
}

@end



#pragma mark AppDelegate

/*
 * Implement principal class: simply override the viewWithFrame method to return the desired demo view.
 */
@implementation AppDelegate

- (Isgl3dView3D *) viewWithFrame:(CGRect)frame {
	return [[[CameraZoomAndFocusDemoView alloc] initWithFrame:frame] autorelease];
}

@end
