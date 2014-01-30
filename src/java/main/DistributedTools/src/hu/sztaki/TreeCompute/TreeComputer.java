/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.TreeCompute;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class TreeComputer {
  public static void main(String[] args) throws Exception {
    String input = args[0];
    String output = args[1];
    
    Configuration conf = new Configuration();
    /*FileSystem fs = FileSystem.get(conf);
    fs.delete(new Path(output), true);*/
    
    Job job = new Job(conf, "Fingerprint tree computer");

    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);

    job.setMapperClass(TreeComputerMap.class);
    job.setReducerClass(TreeComputerReducer.class);
    job.setMapOutputKeyClass(TreeKey.class);
    job.setMapOutputValueClass(FPTreeNode.class);
    
    job.setInputFormatClass(TextInputFormat.class);
    job.setOutputFormatClass(TextOutputFormat.class);
    
    job.setJarByClass(TreeComputer.class);
    FileInputFormat.addInputPath(job, new Path(input));
    FileOutputFormat.setOutputPath(job, new Path(output));
    job.waitForCompletion(false);
  }
}
