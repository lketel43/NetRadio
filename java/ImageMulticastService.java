import java.lang.*;
import java.io.*;
import java.net.*;

public class ImageMulticastService implements Runnable{


    private Streamer stream;

    public ImageMulticastService(Streamer _stream){
        this.stream = _stream;
    }

    public void run(){
        try{
            DatagramSocket dataSocket = new DatagramSocket();
            while(true){
                byte[] image;
                synchronized(this.stream){
                    int nbImages = this.stream.getNbImages();
                    int currentImage = this.stream.getCurrentImage();
                    image = this.stream.getImage(currentImage);
                    if(currentImage + 1 == nbImages) this.stream.setCurrentImage(0);
                    else if(currentImage + 1 < nbImages) this.stream.setCurrentImage(currentImage + 1);
                }
                InetSocketAddress inetSocket = new InetSocketAddress(stream.multiCastAddrImages, stream.multiCastPortImages);
                DatagramPacket packet = new DatagramPacket(image, image.length, inetSocket);
                dataSocket.send(packet);
                Thread.sleep(stream.getImageFrequency());
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }

    }

    
}