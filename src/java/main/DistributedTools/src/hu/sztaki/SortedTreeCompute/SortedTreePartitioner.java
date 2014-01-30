/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.SortedTreeCompute;

import org.apache.hadoop.mapreduce.Partitioner;

public class SortedTreePartitioner extends Partitioner {

  @Override
  public int getPartition(Object key, Object value, int i) {
    TreeKey treeKey = (TreeKey) key;
    long rootNode = treeKey.getRootNode();
    int fpIndex = treeKey.getFpindex();
    return (int) (rootNode + fpIndex) % i;
  }
  
}
