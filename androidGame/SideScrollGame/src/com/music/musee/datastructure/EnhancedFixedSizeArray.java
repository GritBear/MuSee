package com.music.musee.datastructure;

import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;

import com.music.musee.DebugLog;


/**
 * This class is designed for handling overflow capacity exception
 * It uses a secondary storage beyond the T[] provided in the original FixedSizedArray<T>
 * Overflowed elements will be stored in the secondary storage. This adds limited overheads
 * Secondary storage element will shrink to a minimal size when empty
 * 
 * @author Grit
 *
 * @param <T>
 */
public class EnhancedFixedSizeArray<T> extends ABaseArray<T>{
	private static final int CAPACITY_MULTIPLIER = 4;
	private final HybridArray<T> contents;
	
	public EnhancedFixedSizeArray(int size) {
		super(size);
		contents = new HybridArray<T>(size);
	}
	
	public EnhancedFixedSizeArray(int size, Comparator<T> comparator) {
		super(size, comparator);
		contents = new HybridArray<T>(size);
		
	}

	@Override
	public void add(T object) {
		contents.add(object);
	}

	@Override
	public void remove(int index) {
		contents.remove(index);
	}

	@Override
	public T removeLast() {
		T object = contents.get(contents.getCounter()-1);
		contents.remove(contents.getCounter()-1);
		return object;
	}

	@Override
	public void swapWithLast(int index) {
		T objectTemp = contents.get(index);
		contents.set(index, contents.get(contents.getCounter()-1));
		contents.set(contents.getCounter()-1, objectTemp);
	}

	@Override
	public void set(int index, T object) {
		contents.set(index, object);
	}

	@Override
	public void clear() {
		contents.clear();
	}

	@Override
	public T get(int index) {
		return contents.get(index);
	}

	@Override
	public int find(T object, boolean ignoreComparator) {
		 int index = -1;
	        final int count = contents.getCounter();
	    	final boolean sorted = mSorted;
	    	final Comparator<T> comparator = mComparator;
	        if (sorted && !ignoreComparator && count > LINEAR_SEARCH_CUTOFF) {
	            if (comparator != null) {
	                index = Arrays.binarySearch(contents.fixedSizedContent, object, comparator);
	                
	            } else {
	                index = Arrays.binarySearch(contents.fixedSizedContent, object);
	            }
	            // Arrays.binarySearch() returns a negative insertion index if the object isn't found,
	            // but we just want a boolean.
	            if (index < 0) {
	            	//then search the array
	            	index = contents.dynamicSizedContent.indexOf(object);
	            }
	        } else {
	            // unsorted, linear search
	            if (comparator != null && !ignoreComparator) {
	                for (int x = 0; x < count; x++) {
	                	final int result = comparator.compare(contents.get(x), object);
	                    if (result == 0) {
	                        index = x;
	                        break;
	                    } else if (result > 0 && sorted) {
	                    	// we've passed the object, early out
	                    	break;
	                    }
	                }
	            } else {
	                for (int x = 0; x < count; x++) {
	                    if (contents.get(x) == object) {
	                        index = x;
	                        break;
	                    } 
	                }  
	            }
	        }
	        return index;
	}

	@Override
	public void sort(boolean forceResort) {
        if (!mSorted || forceResort) {
            if (mComparator != null) {
                mSorter.sort(contents.fixedSizedContent, contents.fixedSizedContent.length, mComparator);
                Collections.sort(contents.dynamicSizedContent, mComparator);
            } else {
                DebugLog.d("EnhancedFixedSizedArray", "No comparator specified for this type, using Arrays.sort().");
                Arrays.sort(contents.fixedSizedContent, 0, contents.fixedSizedContent.length);
                //cant sort the dynamicSIzedContent
            }
            mSorted = true;
         }
	}

	@Override
	public int getCount() {
		return contents.getCounter();
	}

	@Override
	public int getCapacity() {
		//there is actually no upper limit
		//for performance, it is limited to 4x of the fixed sized array
		return contents.fixedSizedContent.length*CAPACITY_MULTIPLIER;
	}

}
