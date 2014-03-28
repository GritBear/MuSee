package com.music.musee.datastructure;

import java.util.ArrayList;

public class HybridArray<T>{
	private static final int MIN_SECOND_STORAGE_SIZE = 4;
	private int sizeFixedElement;
	//caution! data must be read only!
	public final T[] fixedSizedContent;
	public final ArrayList<T> dynamicSizedContent;
	private int counter;
	
	@SuppressWarnings("unchecked")
	public HybridArray(int fixedSize){
		sizeFixedElement = fixedSize;
		fixedSizedContent = (T[])new Object[sizeFixedElement];
		dynamicSizedContent = new ArrayList<T>(MIN_SECOND_STORAGE_SIZE);
		counter  = 0;
	}
	
	public int getCounter(){return counter;}
	
	public void set(int index, T object){
		assert index >= 0;
		if(index<sizeFixedElement){
			fixedSizedContent[index] = object;
		}else{
			dynamicSizedContent.set(index - sizeFixedElement, object);
		}
	}
	
	public void clear(){
		for(int i = 0; i < sizeFixedElement; i++){
			fixedSizedContent[i] = null;
		}
		dynamicSizedContent.clear();
	}
	
    public void remove(int index) {
		assert index >= 0;
        if (index < counter && index < sizeFixedElement) {
            for (int x = index; x < counter; x++) {
                if (x + 1 < fixedSizedContent.length && x + 1 < counter) {
                	fixedSizedContent[x] = fixedSizedContent[x + 1];
                } else {
                	fixedSizedContent[x]  = null;
                }
            }
            counter--;
        }else if(index < counter){
        	dynamicSizedContent.remove(index -sizeFixedElement);
        	counter--;
        }
    }
	
	public T get(int index){
		assert index >= 0;
		if(index<sizeFixedElement){
			return fixedSizedContent[index];
		}else{
			return dynamicSizedContent.get(index - sizeFixedElement);
		}
	}
	
	public void add(T object){
		if(counter<sizeFixedElement){
			fixedSizedContent[counter] = object;
		}else{
			dynamicSizedContent.add(object);
		}
		counter++;
	}
	
}
