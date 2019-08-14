package com.example.renderscriptgradle64

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        System.loadLibrary("example")
        invoke(cacheDir.toString())
    }

    private external fun invoke(cacheDir: String)
}
