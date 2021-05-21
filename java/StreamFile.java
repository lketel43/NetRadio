import java.io.*;
import java.util.Scanner;

public class StreamFile{

    public static Streamer initStreamerFromFile(String fileName){
        try{
            File file = new File(fileName);
            Scanner scanner = new Scanner(file);
            return new Streamer(scanner.nextLine(), Integer.parseInt(scanner.nextLine()), scanner.nextLine(), Integer.parseInt(scanner.nextLine()), Integer.parseInt(scanner.nextLine()));
        }
        catch(Exception e){
            e.printStackTrace();
            return null;
        }
    }

    public static Pair getManagerInfo(String fileName){
        try{
            File file = new File(fileName);
            Scanner scanner = new Scanner(file);
            String address =  scanner.nextLine();
            int port = Integer.parseInt(scanner.nextLine());
            return new Pair(address, port);
        }
        catch(Exception e){
            e.printStackTrace();
            return null;
        }
    }

    public static void addMessFromFile(Streamer stream, String fileName){
        try{
            File file = new File(fileName);
            Scanner scanner = new Scanner(file);
            String id = scanner.nextLine();
            while(scanner.hasNextLine()){
                stream.addMess(scanner.nextLine(), id);
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

    public static void addImagesFromFile(Streamer stream, String fileName){
        try{
            File file = new File(fileName);
            Scanner scanner = new Scanner(file);
            String id = scanner.nextLine();
            while(scanner.hasNextLine()){
                stream.addImage(scanner.nextLine());
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

}