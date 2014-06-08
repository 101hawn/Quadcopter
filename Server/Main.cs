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
using System.ComponentModel;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.IO.Ports;

namespace Server
{
	class MainClass
	{	
		public static void Main (string[] args)
		{
			

			List<string> ports = new List<string>();

			ScriptManager inputs = new ScriptManager();
			System.Diagnostics.Process.GetCurrentProcess().Exited+= delegate {
				inputs.Quit();
				Console.WriteLine("Exited");
			};
			
		
			
			//Stream outputstream = port.BaseStream;
			//Stream outputstream = File.Open("dummy.output",FileMode.OpenOrCreate);
			string arduino = "";
			foreach(string file in Directory.EnumerateFiles("/dev/"))
			{
				if(file.Contains("ttyACM"))
					arduino = file;
				
			}
			if(arduino == "")
			{
				Console.WriteLine("Please connect arduino");
				Environment.Exit(1);	
			}
			
			Stream outputstream = File.Open(arduino,FileMode.Open);
			//Stream outputstream = Console.OpenStandardOutput();
			Thread.Sleep(5000);
			
			DateTime time = DateTime.Now;
			while(true)
			{
				time +=TimeSpan.FromMilliseconds(100);
				
				Output output = inputs.Update();
				output.Write(outputstream);
				Console.Write(outputstream.ReadByte());
				TimeSpan difference = time-DateTime.Now;
				if(difference.TotalMilliseconds>0)
				Thread.Sleep(difference);
				
			}
			
			
			
			
			
			
		}
		
		
		
	}
	public class ScriptManager
	{
		
		
		List<Process> processes = new List<Process>();
		List<Controller> controllers = new List<Controller>();
		public dynamic Quadcopter = new ExpandoObject();
		public Dictionary<string,Timeout> timeouts = new Dictionary<string, Timeout>();
		ScriptEngine engine;
		public ScriptManager()
		{
			((INotifyPropertyChanged) Quadcopter).PropertyChanged+= HandlePropertyChanged;
			
			engine = Python.CreateEngine();
			
			foreach(string file in Directory.GetFiles("Inputs"))
			{
				if(file.EndsWith(".ipy"))
				{
						ScriptSource source = engine.CreateScriptSourceFromFile(file);
						short priority = 0;
						int lastnumberindex = file.Length-4;
						while(char.IsNumber(file[lastnumberindex])&&lastnumberindex>1)
						      lastnumberindex--;
						
						short.TryParse(file.Substring(lastnumberindex+1,file.Length-lastnumberindex-4),out priority);
						
						try
						{
						controllers.Add(new Controller(source.Compile(),engine,priority,file+".log"));
						}
						catch(Exception)
						{
							
							File.Move(file,file+".corrupt");	
							
							
						}
							
								
						
					
					}
						
						
						
						
					else if (file.EndsWith(".py"))
					
					{
					
					
					
						
					
					ProcessStartInfo info = new ProcessStartInfo("python",file);
					
					info.WorkingDirectory = Directory.GetCurrentDirectory();
					info.RedirectStandardOutput = true;
					info.UseShellExecute = false;
					info.UserName = "root";
					if(info.EnvironmentVariables.ContainsKey("Parent"))
					   info.EnvironmentVariables["Parent"] = Process.GetCurrentProcess().Id.ToString();
					else
					   info.EnvironmentVariables.Add("Parent",Process.GetCurrentProcess().Id.ToString());
					
					System.Diagnostics.Process p = System.Diagnostics.Process.Start(info);
					
					
					p.Exited+= delegate {
						p.Start();
					};
					processes.Add(p);
					p.BeginOutputReadLine();
					
					p.BeginOutputReadLine();
					p.OutputDataReceived+= delegate(object sender, DataReceivedEventArgs e) {
						try
							{
						if(e.Data!=null)
								JsonConvert.PopulateObject(e.Data,Quadcopter);
						p.BeginOutputReadLine();
							}
							catch(Exception ex)
							{
								Console.WriteLine(ex.ToString());
								
							}
					};
					
						
					
				}
				
				
				
			}
			
		
			
		}

		void HandlePropertyChanged (object sender, PropertyChangedEventArgs e)
		{
			if(!e.PropertyName.EndsWith("_timeout"))
			{
				IDictionary<string,object> properties = sender as IDictionary<string,object>;
				if(properties.ContainsKey(e.PropertyName))
				  {
					double timeout = 10000;
					if(properties.ContainsKey(e.PropertyName+"_timeout"))
					{
						object timeoutobject = properties[e.PropertyName+"_timeout"];
						
					
						double.TryParse(timeoutobject.ToString() ,out timeout);
					
					}
					if(!timeouts.ContainsKey(e.PropertyName))
					{
						timeouts.Add(e.PropertyName,new Timeout(properties,e.PropertyName));
						
						
					}
					
					timeouts[e.PropertyName].Run(DateTime.Now.AddMilliseconds(timeout));
				}
			}
		}

		public Output Update()
		{
			Output output = new Output();
		
			output.Pitch = new Value();
			output.Roll = new Value();
			output.Yaw = new Value();
			output.Altitude = new Value();
			
			controllers.Sort();
			
			foreach(Controller c in controllers)
			{
				
				
				try
				{
				c.Run(Quadcopter,output);
				}
				catch(Exception e)
				{
					c.Log(e);
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
		string logfile = "";
		
		public Controller(CompiledCode compiledcode,ScriptEngine engine,short priority)
		{
			scope = engine.CreateScope();
			code = compiledcode;
			operations = engine.CreateOperations();
			code.Execute(scope);
			Update =scope.GetVariableHandle("Update");
			
			Priority = priority;
		}
		public Controller(CompiledCode compiledcode,ScriptEngine engine,short priority,string log)
		{
			scope = engine.CreateScope();
			code = compiledcode;
			operations = engine.CreateOperations();
			code.Execute(scope);
			Update =scope.GetVariableHandle("Update");
			
			Priority = priority;
			
			logfile = log;
		}
		public void Run(dynamic input,Output output)
		{
			try
			{
			operations.Invoke(Update,input,output);
			}
			catch(Exception e)
			{
				Log(e);
				
				
			}
			
			
			
		}
		public int CompareTo(Object obj)
		{
			if(obj.GetType() == typeof(Controller))
				return Priority.CompareTo((obj as Controller).Priority);
			
			return Priority.CompareTo(obj);
			
			
		}
		public void Log(Exception e)
		{
			if(logfile == "")
				return;
			
			
			if(File.Exists(logfile))
			{
				if(File.ReadAllText(logfile).Contains(e.ToString()))
					return;
				
				File.AppendAllText(logfile,e.ToString());
					
			}
			else
			{
				File.WriteAllText(logfile,e.ToString());
				
			}
		
		
			
				
			
		}
			 
			
	 
		
	}
	public struct Output
	{
		
		
		public Value Yaw;
		public Value Pitch; 
		public Value Roll;
		public Value Altitude;
		
		public void Write(Stream str)
		{
			Roll.Result = 500;
			Roll.Kp = 1;
			
			List<byte> bytes = new List<byte>();
			bytes.Add(0x00);			
			bytes.Add(0x11);
			bytes.AddRange(Roll.Serialize());
			bytes.Add(0x00);			
			bytes.Add(0x12);
			bytes.AddRange(Pitch.Serialize());
			bytes.Add(0x00);			
			bytes.Add(0x13);
			bytes.AddRange(Yaw.Serialize());
			bytes.Add(0x00);			
			bytes.Add(0x14);
			bytes.AddRange(Altitude.Serialize());

			try
			{
			str.Write(bytes.ToArray(),0,bytes.Count);
			}
			catch(IOException)
			{
				
			}
		}
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
				
				
				
			}
			
			
		}
		private float kp = float.NaN;
		public float Kp
		{
			set
			{
				if(float.IsNaN(kp))
				{
					kp = value;
				
					
				}
				
			}
			
		}
		private float ki = float.NaN;
		public float Ki
		{
			set
			{
				if(float.IsNaN(ki))
				{
					ki = value;
				
					
				}
				
			}
			
		}
		private float kd = float.NaN;
		public float Kd
		{
			set
			{
				if(float.IsNaN(kd))
				{
					kd = value;
				
					
				}
				
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
		public byte[] Serialize()
		{
			
				
			String str = Convert.ToBase64String(BitConverter.GetBytes((Int16)Result));
			List<byte> bytes = new List<byte>();
			bytes.Add(0x00);
			bytes.Add(0x27);
			foreach(char c in str)
			{
				bytes.AddRange(BitConverter.GetBytes(c));

			}
			bytes.Add(0x00);
			bytes.Add(0x19);
			
			//
			kp=1;
			ki=0;
			kd=59.3f;
			//
			
			
			if(!float.IsNaN(kp))
			{
				bytes.Add(0x00);			
				bytes.Add(0x24);
				bytes.AddRange(Serialize(1000,kp));
			}
			if(!float.IsNaN(ki))
			{
				bytes.Add(0x00);			
				bytes.Add(0x25);
				bytes.AddRange(Serialize(1000,ki));
			}
			if(!float.IsNaN(kd))
			{
				bytes.Add(0x00);			
				bytes.Add(0x26);
				bytes.AddRange(Serialize(1000,kd));
			}
			

			return bytes.ToArray();
			
			
		}
		byte[] Serialize(int K, float f)
		{
			
				
			String str = Convert.ToBase64String(BitConverter.GetBytes((Int16)(f*K)));
			List<byte> bytes = new List<byte>();
			foreach(char c in str)
			{
				bytes.AddRange(BitConverter.GetBytes(c));
				
			}
			bytes.Add(0x00);
			bytes.Add(0x19);
			return bytes.ToArray();
			
			
		}
		
		
		
	}
	public class Timeout
	{
		Thread t = null;
		DateTime timeout;
		String p;
		IDictionary<string,object> dict;
		
		public Timeout(IDictionary<string,object> dictionary, string property)
		{
			p = property;
			dict = dictionary;
			
			
		}
		public void Run(DateTime when)
		{
		
			timeout = when;
			
			if(t!=null&&t.IsAlive)
				t.Interrupt();
			t = new Thread(Check);
			t.Start();
			
			
		}
		public void Check()
		{
			try
			{
				DateTime now = DateTime.Now;
				if((timeout-now).Duration().Milliseconds>10)
				{
					
					Thread.Sleep(timeout-now);
					
					
					now = DateTime.Now;	
					
				}
				
				if(now>timeout)
				{
					dict.Remove(p);
					
				}
				else
				{
					Check();
					
					
				}
			}
		
		
			catch(ThreadInterruptedException)
			{
				
				return	;
			}
					
			
		}
				
				
		
		
	}

	


			         
}
