/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.TreeCompute;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.WritableComparable;


public class TreeKey implements WritableComparable {

  private IntWritable fpIndex_;
  private LongWritable rootNode_;
  
  public TreeKey() {
    fpIndex_ = new IntWritable();
    rootNode_ = new LongWritable();
  }
  
  public TreeKey(int fpIndex, long root) {
    fpIndex_ = new IntWritable(fpIndex);
    rootNode_ = new LongWritable(root);
  }
  
  public int getFpindex() {
    return fpIndex_.get();
  }
  
  public long getRootNode() {
    return rootNode_.get();
  }
  
  @Override
  public void write(DataOutput out) throws IOException {
    fpIndex_.write(out);
    rootNode_.write(out);
  }

  @Override
  public void readFields(DataInput in) throws IOException {
    fpIndex_.readFields(in);
    rootNode_.readFields(in);
  }

  @Override
  public int hashCode() {
    int hash = 7;
    hash = 73 * hash + (this.fpIndex_ != null ? this.fpIndex_.hashCode() : 0);
    hash = 73 * hash + (this.rootNode_ != null ? this.rootNode_.hashCode() : 0);
    return hash;
  }

  @Override
  public int compareTo(Object obj) {
    final TreeKey other = (TreeKey) obj;
    if (this.fpIndex_.get() < other.fpIndex_.get()) {
      return 1;
    } else if (this.fpIndex_.get() > other.fpIndex_.get()) {
      return -1;
    }
    
    if (this.rootNode_.get() < other.rootNode_.get()) {
      return 1;
    } else if (this.rootNode_.get() > other.rootNode_.get()) {
      return -1;
    }
    
    return 0;
  }
}
