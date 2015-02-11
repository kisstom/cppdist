/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.SortedTreeCompute;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableComparable;


public class TreeKey implements WritableComparable<Object> {

  private IntWritable fpIndex_;
  private LongWritable rootNode_;
  private IntWritable treeSize_;
  private IntWritable pathIndex_;
  private LongWritable startNode_;
  private LongWritable endNode_;
  
  public TreeKey() {
    fpIndex_ = new IntWritable();
    rootNode_ = new LongWritable();
    treeSize_ = new IntWritable();
    pathIndex_ = new IntWritable();
    startNode_ = new LongWritable();
    endNode_ = new LongWritable();
  }
  
  public TreeKey(int fp, long r, int size, int pi, long st, long to) {
    fpIndex_ = new IntWritable(fp);
    rootNode_ = new LongWritable(r);
    treeSize_ = new IntWritable(size);
    pathIndex_ = new IntWritable(pi);
    startNode_ = new LongWritable(st);
    endNode_ = new LongWritable(to);
  }
  
  public int getFpindex() {
    return fpIndex_.get();
  }
  
  public long getRootNode() {
    return rootNode_.get();
  }
  
  public int getTreeSize() {
    return treeSize_.get();
  }
  
  public int getPathIndex() {
    return pathIndex_.get();
  }
  
  public long getStartNode() {
    return startNode_.get();
  }
  
  public long getEndNode() {
    return endNode_.get();
  }
  
  @Override
  public void write(DataOutput out) throws IOException {
    fpIndex_.write(out);
    rootNode_.write(out);
    treeSize_.write(out);
    pathIndex_.write(out);
    startNode_.write(out);
    endNode_.write(out);
  }

  @Override
  public void readFields(DataInput in) throws IOException {
    fpIndex_.readFields(in);
    rootNode_.readFields(in);
    treeSize_.readFields(in);
    pathIndex_.readFields(in);
    startNode_.readFields(in);
    endNode_.readFields(in);
  }

  @Override
  public int compareTo(Object o) {
    TreeKey other = (TreeKey) o;
    if (this.fpIndex_.get() > other.fpIndex_.get()) {
      return 1;
    } else if (this.fpIndex_.get() < other.fpIndex_.get()) {
      return -1;
    }
    
    if (this.rootNode_.get() > other.rootNode_.get()) {
      return 1;
    } else if (this.rootNode_.get() < other.rootNode_.get()) {
      return -1;
    }
    
    if (this.treeSize_.get() > other.treeSize_.get()) {
      return 1;
    } else if (this.treeSize_.get() < other.treeSize_.get()) {
      return -1;
    }
    
    if (this.getPathIndex() > other.getPathIndex()) {
      return 1;
    } else if (this.getPathIndex() < other.getPathIndex()) {
      return -1;
    }
    
    if (this.getStartNode() > other.getStartNode()) {
      return 1;
    } else if (this.getStartNode() < other.getStartNode()) {
      return -1;
    }
  
    if (this.getEndNode() > other.getEndNode()) {
      return 1;
    } else if (this.getEndNode() < other.getEndNode()) {
      return -1;
    }
    return 0;
  }

}
