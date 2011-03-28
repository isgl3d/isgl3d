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

#import "Isgl3dBoneBatch.h"
#import "Isgl3dBoneNode.h"
#import "Isgl3dMatrix4D.h"
#import "Isgl3dGLRenderer.h"

@implementation Isgl3dBoneBatch


- (id) initWithNumberOfElements:(unsigned int)numberOfElements andElementOffset:(unsigned int)elementOffset {
    if ((self = [super init])) {
    	_numberOfElements = numberOfElements;
    	_elementOffset = elementOffset;
			
		_frameTransformations = [[NSMutableDictionary alloc] init];
		_currentFrameNumber = 0;
		
		_currentFrameGlobalTransformations = [[NSMutableArray alloc] init];
		_currentFrameGlobalInverseTransformations = [[NSMutableArray alloc] init];
		
		_frameChanged = YES;
    }
	
    return self;
}

- (void) dealloc {
	[_frameTransformations release];
	[_currentFrameGlobalTransformations release];
	[_currentFrameGlobalInverseTransformations release];
	
	[super dealloc];
}

- (void) addBoneTransformations:(NSArray *)transformations forFrame:(unsigned int)frame {
	[_frameTransformations setObject:transformations forKey:[NSNumber numberWithInt:frame]];
}

- (void) setFrame:(unsigned int)frameNumber {
	if (frameNumber != _currentFrameNumber) {
		_currentFrameNumber = frameNumber;
		_frameChanged = YES;
	} else {
		_frameChanged = NO;
	}
}

- (void) setTransformationDirty:(BOOL)isDirty {
	_transformationDirty = isDirty;
}

- (void) updateGlobalTransformation:(Isgl3dMatrix4D *)parentTransformation {
	
	if (_transformationDirty || _frameChanged) {
		[_currentFrameGlobalTransformations removeAllObjects];
		[_currentFrameGlobalInverseTransformations removeAllObjects];
		
		// Get the bone transformations for the current frame
		NSArray * transformations = [_frameTransformations objectForKey:[NSNumber numberWithInt:_currentFrameNumber]];
	
		for (Isgl3dMatrix4D * transformation in transformations) {
			
			Isgl3dMatrix4D * globalTransformation = [Isgl3dMatrix4D matrixFromMatrix:parentTransformation];
			[globalTransformation multiply:transformation];
			[_currentFrameGlobalTransformations addObject:globalTransformation];
			
			Isgl3dMatrix4D * itMatrix = [Isgl3dMatrix4D matrixFromMatrix:globalTransformation];
			[itMatrix invert];
			[itMatrix transpose];
			[_currentFrameGlobalInverseTransformations addObject:itMatrix];
		}	
		
		_transformationDirty = NO;
		_frameChanged = NO;
	}
}


- (void) renderMesh:(Isgl3dGLRenderer *)renderer {
	// Pass transformations to renderer
	[renderer setBoneTransformations:_currentFrameGlobalTransformations andInverseTransformations:_currentFrameGlobalInverseTransformations];
	
	// Render the batch elements
	[renderer render:Triangles withNumberOfElements:_numberOfElements atOffset:_elementOffset];
}


@end
