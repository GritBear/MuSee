/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.music.musee.GL;

import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

import com.music.musee.FixedSizeArray;

/**
 * A 2D rectangular mesh. Can be drawn textured or untextured.
 * This version is modified from the original Grid.java (found in
 * the SpriteText package in the APIDemos Android sample) to support hardware
 * vertex buffers and to insert edges between grid squares for tiling.
 */
public class Grid extends BaseGrid {	
	private static final int quadsLimit = limit/4;
	lookUpKey vertexLookupKey = new lookUpKey();
	FixedSizeArray<BaseGrid> gridArray;
	int maxQuadsAcross = -1;
	int maxVertsAcross = -1;
	int horizontalSegment = 1;

	//this key can be used for both quads and vertexis
	protected class lookUpKey {
		public int gridIndex = -1;
		public int x = -1;
		public int y = -1;
	}
	
	public Grid(int quadsAcross, int quadsDown, boolean useFixedPoint) {
//		super(quadsAcross, quadsDown, useFixedPoint);

		if (quadsDown < 0 || quadsDown >= quadsLimit) {
			throw new IllegalArgumentException("quadsDown");
		}

		//finished calculating number of horizontal segment
		if (quadsAcross * quadsDown >= quadsLimit) {
			maxQuadsAcross = (int) Math.ceil((float)(quadsLimit - 1.0f)/(float)quadsDown);
			horizontalSegment = (int) Math.ceil((float)quadsAcross/(float)maxQuadsAcross);
		}else{
			maxQuadsAcross =  quadsAcross;
			horizontalSegment = 1;
		}
		
		maxVertsAcross = maxQuadsAcross * 2;
		
		final int vertsAcross = quadsAcross * 2;
		final int vertsDown = quadsDown * 2;
		mVertsAcross = vertsAcross;
		mVertsDown = vertsDown;

		//construct the array of grids
		Log.w("lee debug", "max quad across:"+ maxQuadsAcross +";number of grids: " + horizontalSegment);
		gridArray = new FixedSizeArray<BaseGrid>(horizontalSegment);
		for(int i = 0; i < horizontalSegment; i++){
			int horizontalQuadsLeftOver = quadsAcross - maxQuadsAcross * i;
			if(horizontalQuadsLeftOver <= 0){
				//it is the last block
				
				
				
				Log.w("lee debug", "new grid across:"+ (horizontalQuadsLeftOver + maxQuadsAcross) +";down: " + quadsDown);
				gridArray.add(new BaseGrid(horizontalQuadsLeftOver + maxQuadsAcross, quadsDown, useFixedPoint));
			}else{
				//it is the last block
				Log.w("lee debug", "new grid across:"+ maxQuadsAcross +";down: " + quadsDown);
				gridArray.add(new BaseGrid(maxQuadsAcross, quadsDown, useFixedPoint));
			}
		}

		int quadCount = quadsAcross * quadsDown;
		int indexCount = quadCount * 6;
		mIndexCount = indexCount;
		
	}

	//translate the vertex coordinate into the vertex look up key
	private void translateVertexLookUp(int iVertexAcross, int jVertexDown){
		if (iVertexAcross < 0 || iVertexAcross >= mVertsAcross) {
			throw new IllegalArgumentException("iVertexAcross");
		}
		if (jVertexDown < 0 || jVertexDown >= mVertsDown) {
			throw new IllegalArgumentException("jVertexDown");
		}

		//this will write new value to quadLookupKey
		int startOffest = 0;
		int indexX = 0;
		for(int i = 0; i < horizontalSegment; i++){
			//0 to maxVertsAcross - 1 is the first interval with lenght: maxVertsAcross;
			indexX = (iVertexAcross - startOffest);
			if(indexX < maxVertsAcross){
				vertexLookupKey.gridIndex = i;
				vertexLookupKey.x = indexX;
				vertexLookupKey.y = jVertexDown;
				break;
			}
			startOffest += maxVertsAcross;
		}
	}


	@Override
	protected void setVertex(int i, int j, float x, float y, float z, float u, float v){
		translateVertexLookUp(i, j);
		BaseGrid targetGrid = gridArray.get(vertexLookupKey.gridIndex);
		targetGrid.setVertex(vertexLookupKey.x, vertexLookupKey.y, x, y, z, u, v);
	}

//	public void beginDrawingStrips(GL10 gl, boolean useTexture) {
//
//	}
	
	@Override
	public void endDrawing(GL10 gl) {
		for(int i = 0; i < horizontalSegment; i ++){
			gridArray.get(i).endDrawing(gl);
		}
	}

	@Override
	public void drawStrip(GL10 gl, boolean useTexture, int startIndex, int indexCount) {
		Log.w("lee debug", "drawStrip grid called, start:" + startIndex + ";count:"+indexCount);
		int count = indexCount;
		int indexCountStartOffset = 0;
		if (startIndex + indexCount >= mIndexCount) {
			count = mIndexCount - startIndex;
		}		
		//draw all covered region
		for(int i = 0; i < horizontalSegment; i ++){
			BaseGrid tempGrid = gridArray.get(i);
			int curIndexCount = tempGrid.mIndexCount;
			if(indexCountStartOffset <= startIndex && startIndex < curIndexCount + indexCountStartOffset){
				//it should start drawing
				int tempStartIndex = startIndex;
				int tempCount = curIndexCount;
				if(startIndex + count < curIndexCount + indexCountStartOffset){
					tempCount = count;
					tempGrid.drawStrip(gl, useTexture, tempStartIndex, tempCount);
					break; //draw finished here
				}else{
					tempCount = curIndexCount - tempStartIndex;
					tempGrid.drawStrip(gl, useTexture, tempStartIndex, tempCount);
					//update the new start
					startIndex = curIndexCount + indexCountStartOffset;
				}
			}
			indexCountStartOffset += curIndexCount;
		}
	}
	
	@Override
	public void draw(GL10 gl, boolean useTexture) {
//		Log.w("lee debug", "draw grid called");
		for(int i = 0; i < horizontalSegment; i ++){
			gridArray.get(i).draw(gl, useTexture);
		}
    }
	
	@Override
    public void invalidateHardwareBuffers() {
		for(int i = 0; i < horizontalSegment; i ++){
			gridArray.get(i).invalidateHardwareBuffers();
		}
    }
	
    /**
     * Deletes the hardware buffers allocated by this object (if any).
     */
	@Override
    public void releaseHardwareBuffers(GL10 gl) {
		for(int i = 0; i < horizontalSegment; i ++){
			gridArray.get(i).releaseHardwareBuffers(gl);
		}
    }
	
	@Override
    /** 
     * Allocates hardware buffers on the graphics card and fills them with
     * data if a buffer has not already been previously allocated.  Note that
     * this function uses the GL_OES_vertex_buffer_object extension, which is
     * not guaranteed to be supported on every device.
     * @param gl  A pointer to the OpenGL ES context.
     */
	public void generateHardwareBuffers(GL10 gl){
		for(int i = 0; i < horizontalSegment; i ++){
			gridArray.get(i).generateHardwareBuffers(gl);
		}
	}
	
	@Override
    public boolean usingHardwareBuffers() {
        return gridArray.get(0).usingHardwareBuffers();
    }

}
