/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.SortedTreeCompute;

import java.io.ByteArrayInputStream;
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Comparator;
import org.apache.hadoop.io.RawComparator;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;


public class SortedTreeGroupingComparator extends WritableComparator {

  public SortedTreeGroupingComparator() {
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
       
    return 0;
  }

}
