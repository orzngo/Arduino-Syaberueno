import java.io.IOException;
import java.io.OutputStream;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;


public class Driver{

	public static final void main(String[] args){
		String rawMess,mes;
		if(args.length <=0){
			rawMess = "deforuto/me^sse-gi";
		}else{
			rawMess= args[0];
		}
		mes = rawMess + "\r\n";
		try{
			String devPath = System.getenv("SYABERUNO_PATH");
			if(devPath == null){
				throw new Exception("env SYABERUNO_PATH not exported!!\n");
			}
			CommPortIdentifier portID = CommPortIdentifier.getPortIdentifier(devPath);
			SerialPort port = (SerialPort)portID.open("Driver", 1000); 

			port.setSerialPortParams(9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
			port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);

			OutputStream os = port.getOutputStream();
			byte[] data = mes.getBytes();
			Thread.sleep(1500);//なんかここでちょっと待たないと最初が途切れる
			os.write(data);
			os.close();
			port.close();
			System.out.println("\"" + rawMess + "\" sent.\n");
		}catch(NoSuchPortException e){
			System.err.println("Can Not Find Device");
			e.printStackTrace();
		}catch(PortInUseException e){
			System.err.println("Can Not Open Device");
			e.printStackTrace();
		}catch(UnsupportedCommOperationException e){
			System.err.println("Invalid Parameter");
			e.printStackTrace();
		}catch(IOException e){
			e.printStackTrace();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
}

