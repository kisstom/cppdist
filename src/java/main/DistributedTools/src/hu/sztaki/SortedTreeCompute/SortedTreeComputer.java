/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.SortedTreeCompute;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class SortedTreeComputer {
  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    Job job = new Job(conf, "Fingerprint tree computer");
		String input = args[0];
    String output = args[1];
		//job.setInputFormat(SequenceFileInputFormat.class);
		//job.setOutputFormat(SequenceFileOutputFormat.class);
		
		job.setMapOutputKeyClass(TreeKey.class);
		job.setMapOutputValueClass(Edge.class);

		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);

		job.setMapperClass(SortedTreeComputerMap.class);
		job.setReducerClass(SortedTreeComputerReducer.class);

    job.setSortComparatorClass(SortedTreeOutputKeyComparator.class);
		job.setGroupingComparatorClass(SortedTreeGroupingComparator.class);
		job.setPartitionerClass(SortedTreePartitioner.class);

		job.setInputFormatClass(TextInputFormat.class);
    job.setOutputFormatClass(TextOutputFormat.class);
		
    job.setJarByClass(SortedTreeComputer.class);
    job.setNumReduceTasks(4);
    
    FileInputFormat.addInputPath(job, new Path(input));
    FileOutputFormat.setOutputPath(job, new Path(output));
    job.waitForCompletion(false);
  }
}
