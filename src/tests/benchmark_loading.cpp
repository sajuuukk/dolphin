#include <QTest>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTemporaryDir>
#include "kitemviews/kfileitemmodel.h"
#include "kitemviews/kfileitemmodelrolesupdater.h"

// Mock for KFileItemModelRolesUpdater to spy on updates without full UI dependency if needed
// Or we can just use the real one. For now let's try to use the real components.

class LoadingBenchmark : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void benchmarkLargeFolderLoading();

private:
    void createTestFiles(const QString& path, int count);
};

void LoadingBenchmark::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
}

void LoadingBenchmark::createTestFiles(const QString& path, int count)
{
    for (int i = 0; i < count; ++i) {
        QFile f(path + QString("/file_%1.txt").arg(i));
        if (f.open(QIODevice::WriteOnly)) {
            f.write("content");
            f.close();
        }
    }
}

void LoadingBenchmark::benchmarkLargeFolderLoading()
{
    QTemporaryDir tempDir;
    createTestFiles(tempDir.path(), 5000); // Create 5000 files

    KFileItemModel model;

    QBENCHMARK {
        model.slotClear();
        // Simulate loading a directory
        KFileItemList items;
        // In a real scenario, KCoreDirLister would emit itemsAdded.
        // We simulate this by creating KFileItems manually.
        // Doing this inside benchmark might measure item creation too, which is part of the cost.

        items.reserve(5000);
        for(int i=0; i<5000; ++i) {
           items.append(KFileItem(QUrl::fromLocalFile(tempDir.path() + QString("/file_%1.txt").arg(i)), QString(), KFileItem::Unknown));
        }

        model.slotItemsAdded(model.directory(), items);
        model.slotCompleted();
    }
}

QTEST_MAIN(LoadingBenchmark)
#include "benchmark_loading.moc"
