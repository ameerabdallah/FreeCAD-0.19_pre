using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FreeCadCSharp
{
    class MainCmd
    {
        [DllImport("FreeCADApp.dll")]
        public static extern IntPtr GetApplication();
        [DllImport("FreeCADApp.dll")]
        public static extern SortedDictionary<string,string> Config();
        [DllImport("FreeCADApp.dll")]
        public static extern void RunApplication();
        [DllImport("FreeCADApp.dll")]
        public static extern void CloseAllDocuments();
        [DllImport("FreeCADApp.dll")]
        public static extern void Init(int argc, string[] argv);

        static void Main(string []args)
        {
            string sBanner = @"(c) Juergen Riegel, Werner Mayer, Yorik van Havre 2001-2019\n
                         #####                 ####  ###   ####  \n 
                         #                    #      # #   #   # \n
                         #     ##  #### ####  #     #   #  #   # \n
                         ####  # # #  # #  #  #     #####  #   # \n
                         #     #   #### ####  #    #     # #   # \n
                         #     #   #    #     #    #     # #   #  ##  ##  ##\n 
                         #     #   #### ####   ### #     # ####   ##  ##  ##\n\n";

            Config()["ExeName"] = "FreeCAD";
            //Config()["ExeVendor"] = "Team Two";
            //Config()["AppDataSkipVendor"]= "true";

            //Set the banner 
            //Config()["CopyrightInfo"] = sBanner;

            try
            {
                //Config()["RunMode"] = "Exit";
                //Config()["LoggingConsole"] = "1";

                //Init the Application
                string[] fakeArgs = { "FreeCadNet.exe" }; 
                Init(fakeArgs.Length, fakeArgs);
            } catch
            {
                //TODO: Stuff for error
            }

            //Run Phase:: --- 
            try
            {
                RunApplication();
            } catch
            {
                //TODO: Stuff For Error
            }

            //Destruction Phase
            try
            {
                CloseAllDocuments();
            }
            catch (Exception e)
            {
                Console.WriteLine("Uhhhhhhh");
            }
        }
    }
}

