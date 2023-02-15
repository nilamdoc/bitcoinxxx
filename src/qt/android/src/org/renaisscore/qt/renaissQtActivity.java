package org.renaisscore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class RenaissQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File renaissDir = new File(getFilesDir().getAbsolutePath() + "/.renaiss");
        if (!renaissDir.exists()) {
            renaissDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
