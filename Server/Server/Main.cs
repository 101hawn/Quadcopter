using System;
using System.Dynamic;
using System.Diagnostics;
using System.Collections.Generic;
using System.Collections;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.IO;
using System.Threading;

namespace Server
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Inputs inputs = new Inputs();
			dynamic quadcopter =  inputs.Quadcopter;
			while(true)
			{
				try{
				Console.WriteLine(quadcopter.Stuff);
				}catch(Exception)
				{
				}
				Thread.Sleep(10);
			}
			System.Diagnostics.Process.GetCurrentProcess().Exited+= delegate {
				inputs.Quit();
			};
			
			
		}
		
	}
	public class Inputs
	{
		List<Process> processes = new List<Process>();
		public dynamic Quadcopter = new ExpandoObject();
		
		public Inputs()
		{
			
			foreach(string file in Directory.GetFiles("Inputs"))
			{
				if(file.EndsWith(".py"))
				{
					ProcessStartInfo info = new ProcessStartInfo("python",file);
					
					info.WorkingDirectory = Directory.GetCurrentDirectory();
					info.RedirectStandardOutput = true;
					info.UseShellExecute = false;
					System.Diagnostics.Process p = System.Diagnostics.Process.Start(info);
					processes.Add(p);
					p.BeginOutputReadLine();
					
					p.BeginOutputReadLine();
					p.OutputDataReceived+= delegate(object sender, DataReceivedEventArgs e) {
						if(e.Data!=null)
							MergeJObject((JObject)JsonConvert.DeserializeObject(e.Data));
						p.BeginOutputReadLine();
					};
						
					
				}
				
				
				
			}
			
		
			
		}
		
		void MergeJObject(JObject obj)
		{
			var dict = (IDictionary<string,object>) Quadcopter;
		
			foreach(var j in obj)
			{
				if(!dict.ContainsKey(j.Key))
					dict.Add(j.Key,j.Value);
				else
					dict[j.Key] = j.Value;
				
				
				
				
			}
			
			
			
			
			
		}
		public void Quit()
		{
			foreach(Process p in processes)
				p.Close();
			
			
		}
		
		
		
	}
}
