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

#import "Isgl3dObject3DGrabber.h"
#import "Isgl3dColorUtil.h"
#import "Isgl3dGLObject3D.h"

@implementation Isgl3dObject3DGrabber

static Isgl3dObject3DGrabber * _instance;

- (id) init {
	if ((self = [super init])) {
		_activeObjects = [[NSMutableDictionary alloc] init];
		_objectCount = 0;
		
	}
	
	return self;
}

- (void) dealloc {
	
	[_activeObjects release]; 
	
    [super dealloc];
}


+ (Isgl3dObject3DGrabber *) sharedInstance {
	@synchronized (self) {
		if (!_instance) {
			_instance = [[Isgl3dObject3DGrabber alloc] init];
		}
	}
	return _instance;
}

+ (void) resetInstance {
	if (_instance) {
		[_instance release];
		_instance = nil;
	}
}

- (void) startCapture {
	_objectCount = 0;
	[_activeObjects removeAllObjects];
	
}

- (void) getCaptureColor:(float *)color forObject:(Isgl3dGLObject3D *)object {
	_objectCount++;
	
	NSString * colorString = [NSString stringWithFormat:@"0x%06X", _objectCount];
//	NSLog(@"colorString = %@", colorString);
	
//	Uncomment to see capture render
//	[Isgl3dColorUtil hexColorStringToFloatArray:@"FF0000" floatArray:color];
	[Isgl3dColorUtil hexColorStringToFloatArray:colorString floatArray:color];
	
//	NSLog(@"color = (%f, %f, %f)",color[0], color[1], color[2]);
	
	[_activeObjects setObject:object forKey:colorString];
}

- (Isgl3dGLObject3D *) getObjectWithColorString:(NSString *)colorString {
	return [_activeObjects objectForKey:colorString];
}


@end
