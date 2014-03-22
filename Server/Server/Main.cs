using System;
using System.Dynamic;
using System.Diagnostics;
using System.Collections.Generic;
using System.Collections;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.IO;
using System.Threading;
using IronPython.Hosting;
using Microsoft.Scripting;
using Microsoft.Scripting.Hosting;
using System.Runtime.Remoting;
namespace Server
{
	class MainClass
	{	
		public static void Main (string[] args)
		{
			
			
			
			ScriptManager inputs = new ScriptManager();
			dynamic quadcopter =  inputs.Quadcopter;
			while(true)
			{
				
				Output output = inputs.Update();
				Console.WriteLine(output.Pitch.Result);
				Console.WriteLine(output.Roll.Result);
				Console.WriteLine(output.Yaw.Result);
				Console.WriteLine();
					
				
				Thread.Sleep(10);
			}
			System.Diagnostics.Process.GetCurrentProcess().Exited+= delegate {
				inputs.Quit();
			};
			
			
			
			
			
		}
		
	}
	public class ScriptManager
	{
		List<Process> processes = new List<Process>();
		List<Controller> controllers = new List<Controller>();
		public dynamic Quadcopter = new ExpandoObject();
		ScriptEngine engine;
		ScriptScope scope;
		public ScriptManager()
		{
			engine = Python.CreateEngine();
			scope = engine.CreateScope();
			
			foreach(string file in Directory.GetFiles("Inputs"))
			{
				if(file.EndsWith(".py"))
				{
					if(!File.ReadAllText(file).Contains("json"))
					{
						ScriptSource source = engine.CreateScriptSourceFromFile(file);
						short priority = 0;
						int lastnumberindex = file.Length-4;
						while(char.IsNumber(file[lastnumberindex])&&lastnumberindex>1)
						      lastnumberindex--;
						
						short.TryParse(file.Substring(lastnumberindex+1,file.Length-lastnumberindex-4),out priority);
						
						try
						{
						controllers.Add(new Controller(source.Compile(),engine,priority));
						}
						catch(Exception)
						{
							
							File.Move(file,file+".corrupt");	
							
							
						}
							
								
						
						
					}
						
						
						
						
					else
					
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
						try
							{
						if(e.Data!=null)
								JsonConvert.PopulateObject(e.Data,Quadcopter);
							//MergeJObject((JObject)JsonConvert.DeserializeObject(e.Data));
						p.BeginOutputReadLine();
							}
							catch(Exception)
							{
								
							File.Move(file,file+".corrupt");	
								
								
							}
					};
					}
						
					
				}
				
				
				
			}
			
		
			
		}

		public Output Update()
		{
			Output output = new Output();
			output.Pitch = new Value();
			output.Roll = new Value();
			output.Yaw = new Value();
			
			controllers.Sort();
			
			foreach(Controller c in controllers)
			{
				if(output.Pitch.IsBounded()&&output.Roll.IsBounded()&&output.Yaw.IsBounded())
					break;
				
				try
				{
				c.Run(Quadcopter,output);
				}
				catch(Exception e)
				{
				}
			}
			return output;
			
		}
		public void Quit()
		{
			foreach(Process p in processes)
				p.Close();
			
			
		}
		
		
		
	}
	public class Controller:IComparable
	{
		ScriptScope scope;
		CompiledCode code;
		ObjectOperations operations;
		ObjectHandle Update;
		public short Priority;
		
		public Controller(CompiledCode compiledcode,ScriptEngine engine,short priority)
		{
			scope = engine.CreateScope();
			code = compiledcode;
			operations = engine.CreateOperations();
			code.Execute(scope);
			Update =scope.GetVariableHandle("Update");
			
			Priority = priority;
		}
		public void Run(dynamic input,Output output)
		{
			operations.Invoke(Update,input,output);
			
			
			
			
		}
		public int CompareTo(Object obj)
		{
			if(obj.GetType() == typeof(Controller))
				return Priority.CompareTo((obj as Controller).Priority);
			
			return Priority.CompareTo(obj);
			
			
		}
			 
			
	 
		
	}
	public struct Output
	{
		
		
		public Value Yaw;
		public Value Pitch; 
		public Value Roll;
		
		
	}
	public class Value
	{
		public float Min
		{
			set
			{
				if(value<=max)
				{
					if(value>min)
						min = value;
				}
				
				
			}
			get
			{
				return min;
				
				
			}
			
			
		}
		float min;
		
		public float Max
		{
			set
			{
				
				if(value>=min)
				{	
				
					if(value<max)
						max = value;
				}
				
			}
			get
			{
				return max;
				
				
			}
			
			
		}
		float max;

		public float Result
		{
			set
			{
				Max = value;
				Min = value;
				
				
			}
			get
			{
				if(0>min&&0<max)
					return 0;
				else
				{
					if(Math.Abs(min)<Math.Abs(max))
					   return min;
					else
					   return max;
					   
			
					
					
				}
				
				return min;
				
				
			}
			
			
		}
		
		public Value()
		{
			min = float.NegativeInfinity;
			max = float.PositiveInfinity;
			
			
		}
		public bool IsBounded()
		{
			if(Min==Max&&Min==Result)
				return true;
			return false;
			
			
			
		}
		
		
		
		
	}
}
