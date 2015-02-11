/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.SortedTreeCompute;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;



public class SortedTreeOutputKeyComparator extends WritableComparator {

  public SortedTreeOutputKeyComparator() {
    super(TreeKey.class, true);
  }

  @Override
  public int compare(WritableComparable o1, WritableComparable o2) {
    TreeKey key1 = (TreeKey) o1;
    TreeKey key2 = (TreeKey) o2;
    
    if (key1.getFpindex() > key2.getFpindex()) {
      return 1;
    } else if (key1.getFpindex() < key2.getFpindex()) {
      return -1;
    }
    
    if (key1.getRootNode() > key2.getRootNode()) {
      return 1;
    } else if (key1.getRootNode() < key2.getRootNode()) {
      return -1;
    }
    
    if (key1.getTreeSize() > key2.getTreeSize()) {
      return 1;
    } else if (key1.getTreeSize() < key2.getTreeSize()) {
      return -1;
    }
    
    if (key1.getPathIndex() > key2.getPathIndex()) {
      return 1;
    } else if (key1.getPathIndex() < key2.getPathIndex()) {
      return -1;
    }
    
    if (key1.getStartNode() > key2.getStartNode()) {
      return 1;
    } else if (key1.getStartNode() < key2.getStartNode()) {
      return -1;
    }
  
    if (key1.getEndNode() > key2.getEndNode()) {
      return 1;
    } else if (key1.getEndNode() < key2.getEndNode()) {
      return -1;
    }
    
    return 0;
  }

}
